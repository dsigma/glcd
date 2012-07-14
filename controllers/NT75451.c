#if defined(GLCD_CONTROLLER_NT75451)

#include "../glcd.h"

void glcd_command(uint8_t c)
{
	GLCD_RS_LOW();
	glcd_parallel_write(c);
	GLCD_RS_HIGH();	
}

void glcd_data(uint8_t c)
{
	GLCD_RS_HIGH();
	glcd_parallel_write(c);
	GLCD_RS_LOW();
}

void glcd_set_contrast(uint8_t val) {
}

void glcd_power_down(void)
{
}

void glcd_power_up(void)
{
}

void glcd_set_y_address(uint8_t y)
{
	/** Code by NGX Technologies */
	glcd_command(0xB0 | (y > GLCD_NUMBER_OF_BANKS ? GLCD_NUMBER_OF_BANKS : y));
}

void glcd_set_x_address(uint8_t x)
{
	/** Code by NGX Technologies */
	uint8_t lsb, msb;
	
	msb	= (((x & 0xF0) >> 4)| 0x10);
	lsb	= (x & 0x0F);
	
	glcd_command(lsb);
	glcd_command(msb);	
}

/* Write screen buffer to display, within bounding box only */
void glcd_write()
{
	uint8_t bank;
	
	for (bank = 0; bank < GLCD_NUMBER_OF_BANKS; bank++) {
		/* Each bank is a single row 8 bits tall */
		uint8_t column;		
		
		if (glcd_bbox_selected->y_min >= (bank+1)*8) {
			continue; /* Skip the entire bank */
		}
		
		if (glcd_bbox_selected->y_max < bank*8) {
			break;    /* No more banks need updating */
		}
		
		glcd_set_y_address(bank);
		glcd_set_x_address(glcd_bbox_selected->x_min);

		for (column = glcd_bbox_selected->x_min; column <= glcd_bbox_selected->x_max; column++)
		{
			glcd_data( glcd_buffer_selected[GLCD_NUMBER_OF_COLS * bank + column] );
		}
	}

	/* Display updated, we can reset the bounding box */
	glcd_reset_bbox();
	
}

#endif
