#include <lcom/lcf.h>

#include "videocard.h"

vbe_mode_info_t vmi_p;
int r;
struct minix_mem_range mr; /* physical memory range */
static unsigned vram_base; /* VRAM  physical addresss */
static unsigned	vram_size; /* VRAM size */
static void* video_mem;
static uint8_t bits_per_pixel;
static uint8_t bytes_per_pixel;
static uint8_t RedMaskSize, RedFieldPosition, GreenMaskSize, GreenFieldPosition, BlueMaskSize, BlueFieldPosition;
static uint16_t horizontal_res;
static uint16_t vertical_res;


int (set_mode)(uint16_t mode){
    reg86_t r;
    memset(&r, 0, sizeof(r));

    r.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
    r.bx = 1<<14|mode; // set bit 14: linear framebuffer
    r.intno = VIDEO_CARD;

    if( sys_int86(&r) != OK ) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return 1;
    }
    return 0;
}

void* (vg_init)(uint16_t mode){
    if (vbe_get_mode_info(mode, &vmi_p) != 0){ 
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

    map_mem();
    
    if(set_mode(mode)!=0) return NULL;
    
    return video_mem;
}

void (map_mem)(){

    /* Allow memory mapping */
    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    
    vram_base = vmi_p.PhysBasePtr;
  	vram_size = horizontal_res * vertical_res * bytes_per_pixel;
    
    /* Map memory */
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if(video_mem == MAP_FAILED)
        panic("couldn’t map video memory");
}

int drawPixel(uint16_t x,uint16_t y, uint32_t color){
//    if (x >= horizontal_res || y >= vertical_res)
//		return 1;
    uint8_t *adr = video_mem;
    adr += (horizontal_res * y * bytes_per_pixel); //Número de bytes que se percorre até ao ponto (x,y) 
  	adr += (x * bytes_per_pixel);

    for (size_t i = 0; i < bytes_per_pixel; i++) { //Repeat the process until you finish the intire pixel
		*adr = (color >> (i * 8)); //Change color of a certain adress
		adr++;
	}
    return 0;
}

int (vg_draw_hline)(uint16_t x,uint16_t y,uint16_t len ,uint32_t color){
//    if (x >= horizontal_res || y >= vertical_res)
//		return 1;
    //Change color of pixel in line
    for (size_t i = 0; i < len; i++)
	{
        if(x+i>horizontal_res) break;
		drawPixel(x+i,y,color);
  	}
    return 0;
}

int (vg_draw_rectangle)(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint32_t color){
    //Draw line by line (horizontal)
    for(size_t i=0; i<height; i++){
        if(y+i>vertical_res) break;
        if(vg_draw_hline(x,y+i,width,color)!=0){
            printf("Could not draw line!");
            return 1;
        }
    }

    return 0;
}


