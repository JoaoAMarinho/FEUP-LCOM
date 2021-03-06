#include <lcom/lcf.h>

#include "Videocard.h"

static void* video_mem;
static void *secondBuffer;

vbe_mode_info_t vmi_p;
int r1;
struct minix_mem_range mr; /* physical memory range */
static unsigned vram_base; /* VRAM  physical addresss */
static unsigned	vram_size; /* VRAM size */
static uint8_t RedMaskSize, RedFieldPosition, GreenMaskSize, GreenFieldPosition, BlueMaskSize, BlueFieldPosition;
extern uint16_t horizontal_res, vertical_res;
static uint8_t bits_per_pixel, bytes_per_pixel;

//---------------------------------------------------------------------------------------------

int (set_mode)(uint16_t mode){
    reg86_t reg;
    memset(&reg, 0, sizeof(reg));

    reg.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
    reg.bx = BIT(14)|mode; // set bit 14: linear framebuffer
    reg.intno = VIDEO_CARD;

    if( sys_int86(&reg) != OK ) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return 1;
    }
    return 0;
}

void* (vg_init)(uint16_t mode){

    if(map_mem(mode)==NULL) return NULL;
    
    if(set_mode(mode)!=0) return NULL;

    return video_mem;
}

int get_vbe_mode_info(uint16_t mode, vbe_mode_info_t *vmi_p){
    mmap_t map;
    reg86_t reg;

    memset(&reg, 0, sizeof(reg)); // Reset the construction

    // Allocate the low memory block
    if (lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL) {
        printf("failed alloc\n");
        return 1;
    }

     // Get the mode information
    phys_bytes buf = map.phys;

    reg.ax = 0x4F01;        // VBE call, function 01
    reg.es = PB2BASE(buf);
    reg.di = PB2OFF(buf);
    reg.cx = mode;
    reg.intno = VIDEO_CARD;

    if (sys_int86(&reg) != OK) {
        panic("vbe_get_mode_info: sys_int86() failed \n");
  }

  // Put the memory buffer with the function returned data in vmi_p
  *vmi_p = *(vbe_mode_info_t *) (map.virt);
  lm_free(&map);

  return 0;
}

void* (map_mem)(uint16_t mode){
    if (get_vbe_mode_info(mode, &vmi_p) != 0){ 
    	printf("Failed to get mode \n");
    	return NULL;
  	}
    
    horizontal_res=vmi_p.XResolution;
    vertical_res = vmi_p.YResolution;

    bits_per_pixel = vmi_p.BitsPerPixel;
	bytes_per_pixel = (bits_per_pixel+7)/8;

	RedMaskSize = vmi_p.RedMaskSize;
	GreenMaskSize = vmi_p.GreenMaskSize;
	BlueMaskSize = vmi_p.BlueMaskSize;

	RedFieldPosition = vmi_p.RedFieldPosition;
	GreenFieldPosition = vmi_p.GreenFieldPosition;
	BlueFieldPosition = vmi_p.BlueFieldPosition;

    vram_base = vmi_p.PhysBasePtr;
  	vram_size = horizontal_res * vertical_res * bytes_per_pixel;

    // Allow memory mapping 
    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    if( OK != (r1 = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r1);
    
    
    // Map memory 
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(video_mem == MAP_FAILED)
        panic("couldn’t map video memory");
    
    secondBuffer = malloc(vram_size);
    
    return video_mem;
}

int drawPixel(uint16_t x,uint16_t y, uint32_t color){
    if (x >= horizontal_res || y >= vertical_res)
		return 1;
    uint8_t *adr = secondBuffer;
    adr += (horizontal_res * y * bytes_per_pixel); //Número de bytes que se percorre até ao ponto (x,y) 
  	adr += (x * bytes_per_pixel);

    for (int i = 0; i < bytes_per_pixel; i++) { //Repeat the process until you finish the intire pixel
		*adr = (color >> (i * 8)); //Change color of a certain adress
		adr++;
	}
    return 0;
}

void copy_to_vram(){
    memcpy(video_mem, secondBuffer, vertical_res * horizontal_res * bytes_per_pixel);
}

void free_mem(){
    free(secondBuffer);
}
