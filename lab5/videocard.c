#include <lcom/lcf.h>

#include "videocard.h"

vbe_mode_info_t vmi_p;
int r1;
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

void* (map_mem)(uint16_t mode){
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
    
    return video_mem;
}

int drawPixel(uint16_t x,uint16_t y, uint32_t color){
    if (x >= horizontal_res || y >= vertical_res)
		return 1;
    char *adr = video_mem;
    adr += (horizontal_res * y * bytes_per_pixel); //Número de bytes que se percorre até ao ponto (x,y) 
  	adr += (x * bytes_per_pixel);

    for (int i = 0; i < bytes_per_pixel; i++) { //Repeat the process until you finish the intire pixel
		*adr = (color >> (i * 8)); //Change color of a certain adress
		adr++;
	}
    return 0;
}

int (vg_draw_hline)(uint16_t x,uint16_t y,uint16_t len ,uint32_t color){
    if (x >= horizontal_res || y >= vertical_res)
		return 1;
    //Change color of pixel in line
    for (int i = 0; i < len; i++)
	{
        if(x+i>horizontal_res) break;

		drawPixel(x+i,y,color);
  	}
    return 0;
}

int (vg_draw_rectangle)(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint32_t color){
    //Draw line by line (horizontal)
    for(int i=0; i<height; i++){
        if(y+i>vertical_res) break;

        vg_draw_hline(x,y+i,width,color);
    }

    return 0;
}

uint16_t getHorizontal() {
	return horizontal_res;
}

uint16_t getVertical() {
	return vertical_res;
}

void get_color(uint32_t *color, unsigned row, unsigned column, uint8_t no_rectangles, uint32_t first, uint8_t step){
    if (bits_per_pixel == 8) { // Indexed mode
		*color = (first + (row * no_rectangles + column) * step) % (1 << bits_per_pixel);
	}
    else // Direct mode
	{
		uint8_t red_first = get_color_bits(first, RedMaskSize, RedFieldPosition);
		uint8_t green_first = get_color_bits(first, GreenMaskSize, GreenFieldPosition);
		uint8_t blue_first = get_color_bits(first, BlueMaskSize, BlueFieldPosition);

		uint32_t red = (red_first + column * step) % (1 << RedMaskSize);
		uint32_t green = (green_first + column * step) % (1 << GreenMaskSize);
		uint32_t blue = (blue_first + column * step) % (1 << BlueMaskSize);

		*color = (red << RedFieldPosition) | (green << GreenFieldPosition) | (blue << BlueFieldPosition);
	}
}

uint8_t get_color_bits(uint32_t color, unsigned mask_size, unsigned field_position) { //Returns the bits of a certain color in the field_position
	uint32_t temp = color >> field_position;                                          //with size(mask_size).

	uint32_t mask = pow(2, mask_size) - 1;

	return (uint8_t)(temp & mask);
}

