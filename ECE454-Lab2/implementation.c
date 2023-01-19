#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"  // DO NOT REMOVE this line
#include "implementation_reference.h"   // DO NOT REMOVE this line
#include <stdlib.h>
// #pragma GCC target ("avx,avx2")
// #include <immintrin.h>

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image up
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveUp(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    return processMoveUpReference(buffer_frame, width, height, offset);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image left
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveRight(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    return processMoveRightReference(buffer_frame, width, height, offset);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image up
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveDown(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    return processMoveDownReference(buffer_frame, width, height, offset);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image right
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveLeft(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    return processMoveLeftReference(buffer_frame, width, height, offset);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param rotate_iteration - rotate object inside frame buffer clockwise by 90 degrees, <iteration> times
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note: You can assume the frame will always be square and you will be rotating the entire image
 **********************************************************************************************************************/
unsigned char *processRotateCW(unsigned char *buffer_frame, unsigned width, unsigned height,
                               int rotate_iteration) {
    return processRotateCWReference(buffer_frame, width, height, rotate_iteration);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param rotate_iteration - rotate object inside frame buffer counter clockwise by 90 degrees, <iteration> times
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note: You can assume the frame will always be square and you will be rotating the entire image
 **********************************************************************************************************************/
unsigned char *processRotateCCW(unsigned char *buffer_frame, unsigned width, unsigned height,
                                int rotate_iteration) {
    return processRotateCCWReference(buffer_frame, width, height, rotate_iteration);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/
unsigned char *processMirrorX(unsigned char *buffer_frame, unsigned int width, unsigned int height, int _unused) {
    return processMirrorXReference(buffer_frame, width, height, _unused);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/
unsigned char *processMirrorY(unsigned char *buffer_frame, unsigned width, unsigned height, int _unused) {
    return processMirrorYReference(buffer_frame, width, height, _unused);
}

/***********************************************************************************************************************
 * WARNING: Do not modify the implementation_driver and team info prototype (name, parameter, return value) !!!
 *          Do not forget to modify the team_name and team member information !!!
 **********************************************************************************************************************/
void print_team_info(){
    // Please modify this field with something interesting
    char team_name[] = "TTGH";

    // Please fill in your information
    char student_first_name[] = "Won-Jin";
    char student_last_name[] = "Kim";
    char student_student_number[] = "1003611424";

    // Printing out team information
    printf("*******************************************************************************************************\n");
    printf("Team Information:\n");
    printf("\tteam_name: %s\n", team_name);
    printf("\tstudent_first_name: %s\n", student_first_name);
    printf("\tstudent_last_name: %s\n", student_last_name);
    printf("\tstudent_student_number: %s\n", student_student_number);
}

/***********************************************************************************************************************
 * WARNING: Do not modify the implementation_driver and team info prototype (name, parameter, return value) !!!
 *          You can modify anything else in this file
 ***********************************************************************************************************************
 * @param sensor_values - structure stores parsed key value pairs of program instructions
 * @param sensor_values_count - number of valid sensor values parsed from sensor log file or commandline console
 * @param frame_buffer - pointer pointing to a buffer storing the imported  24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param grading_mode - turns off verification and turn on instrumentation
 ***********************************************************************************************************************
 *
 **********************************************************************************************************************/

typedef struct 
{
    int row, col;
    unsigned char rgb[3];
} Pixel;



// typedef enum
//     {
//         W, S, A, D, CW, CCW, MX, MY
//     }SensorEnum;
    
//     const static struct
//     {
//         const char* str;
//         SensorEnum key;
//     } key2EnumConv[] = {
//         {"W", W}, {"S", S},{"A", A},{"D", D},{"CW", CW}, {"CCW", CCW}, {"MX", MX}, {"MY", MY}
//     };

// SensorEnum Key2Enum(const char* str)
// {
//     for(int i=0; i<sizeof(key2EnumConv)/sizeof(key2EnumConv[0]); ++i)
//     {
//         if(!strcmp(str, key2EnumConv[i].str))
//         {
//             return key2EnumConv[i].key;
//         }
//     }
// }


Pixel pixel_with_color[10000*10000];


void implementation_driver(struct kv *sensor_values, int sensor_values_count, unsigned char *frame_buffer,
                           unsigned int width, unsigned int height, bool grading_mode) {
    int processed_frames = 0;
    
    register int color_pixel_cnt = 0;
    int temp_pos = 0;

    register Pixel* val1; register Pixel* val2; register Pixel* val3; 
    register Pixel*  val4; register Pixel* val5; register Pixel*  val6; 
    register Pixel*  val7; register Pixel*  val8; register Pixel*  val9;
    register Pixel*  val10; register Pixel*  val11; register Pixel*  val12;
    unsigned char * aa; 

    // int row_size = (width<<1) + width;
    // char * white_row = (char*) malloc(row_width);
    // rgb* frame = (rgb*) frame_buffer;
    unsigned char white_pixels[3] = {0xff, 0xff, 0xff};
    // register Pixel* temp = &pixel_with_color[0];
    // register unsigned char *frame = &frame_buffer[0];
    for(int row=0; row<height; ++row)
    {
        for(int col=0; col<width; ++col)
        {
            if(frame_buffer[temp_pos] != 255 || 
               frame_buffer[temp_pos + 1] != 255 || 
               frame_buffer[temp_pos + 2] != 255)
            // if(memcmp(&frame_buffer[temp_pos], &white_pixels ,sizeof(white_pixels)))
            {
                pixel_with_color[color_pixel_cnt].row = row;
                pixel_with_color[color_pixel_cnt].col = col;
                pixel_with_color[color_pixel_cnt].rgb[0] = frame_buffer[temp_pos];
                pixel_with_color[color_pixel_cnt].rgb[1] = frame_buffer[temp_pos+1];
                pixel_with_color[color_pixel_cnt].rgb[2] = frame_buffer[temp_pos+2];
                
                // memcpy(pixel_with_color[color_pixel_cnt].rgb, &frame_buffer[temp_pos] , sizeof(pixel_with_color[color_pixel_cnt].rgb));
                color_pixel_cnt += 1;
            }
            temp_pos += 3;
            // frame += 9;
        }
    }
    // printf("%d\n", color_pixel_cnt);
    


    int dir_transform[4] = {0,}; //0: up, 1: left, 2: down, 3: right
    int rot_transform = 0;
    bool xMirrored = false;
    bool yMirrored = false;

    int current_val = 0;
    int idx = 0;

    register int render_col = width - 1;
    register int render_row = 0;
    register Pixel* temp = &pixel_with_color[0];
    register unsigned char *frame = &frame_buffer[0];

    for (int sensorValueIdx = 0; sensorValueIdx < sensor_values_count; sensorValueIdx++) 
    {
        // SensorEnum current_key = Key2Enum(sensor_values[sensorValueIdx].key);
        const char* current_key = sensor_values[sensorValueIdx].key;
        current_val = sensor_values[sensorValueIdx].value;
        switch(current_key[0]) 
        {   
            case 'W':
                idx = !xMirrored ? 0 : 2;
                dir_transform[(idx-rot_transform) & 3] += current_val;
                break;
            case 'S':
                idx = !xMirrored ? 2 : 0;
                dir_transform[(idx-rot_transform) & 3] += current_val;
                break;
            case 'A':
                idx = !yMirrored ? 3 : 1;
                dir_transform[(idx-rot_transform) & 3] += current_val;
                break;
            case 'D':
                idx = !yMirrored ? 1 : 3;
                dir_transform[(idx-rot_transform) & 3] += current_val;
                break;
            case 'C':
                if(xMirrored && yMirrored)
                {
                    current_val *= -1;
                    xMirrored = yMirrored = false;
                }
                else if(xMirrored || yMirrored)
                {
                    current_val *= -1;
                }
                if(current_key[1] != 'W')
                {
                    current_val *= -1;
                }
                rot_transform += current_val;
                break;
            
            case 'M':
                if (current_key[1] == 'X')
                {
                    xMirrored = !xMirrored;

                    // if(xMirrored)
                    // {
                    //     xMirrored = false;
                    // } else {
                    //     xMirrored = true;
                    // }
                } else {
                    yMirrored = !yMirrored;
                    // if(yMirrored)
                    // {
                    //     yMirrored = false;
                    // } else {
                    //     yMirrored = true;
                    // }
                }
                break;
        }
        
        processed_frames += 1;
        if (processed_frames % 25 == 0) {
            // for(int i=0; i<color_pixel_cnt; ++i)
            // {
            //     int idx = (pixel_with_color[i].row * width + pixel_with_color[i].col) * 3;
            //     memset(frame_buffer+idx, 255, 3);
            // }
            memset(frame_buffer, 255, height*width*3);
            int vertical = dir_transform[2] - dir_transform[0];
            int horizontal = dir_transform[1] - dir_transform[3];
    
            if(vertical != 0)
            {
                for(int i=0; i<color_pixel_cnt; i+=12)
                {
                    val1 = temp + i;
                    val2 = temp + i+1;
                    val3 = temp + i+2;
                    val4 = temp + i+3;
                    val5 = temp + i+4;
                    val6 = temp + i+5;
                    val7 = temp + i+6;
                    val8 = temp + i+7;
                    val9 = temp + i+8;
                    val10 = temp + i+9;
                    val11 = temp + i+10;
                    val12 = temp + i+11;
                    
                    val1->row += vertical;
                    val2->row += vertical;
                    val3->row += vertical;
                    val4->row += vertical;
                    val5->row += vertical;
                    val6->row += vertical;
                    val7->row += vertical;
                    val8->row += vertical;
                    val9->row += vertical;
                    val10->row += vertical;
                    val11->row += vertical;
                    val12->row += vertical;

                    // pixel_with_color[i].row += vertical;
                }
            }
            if (horizontal != 0)
            {
                for(int i=0; i<color_pixel_cnt; i+=12)
                {
                    val1 = temp + i;
                    val2 = temp + i+1;
                    val3 = temp + i+2;
                    val4 = temp + i+3;
                    val5 = temp + i+4;
                    val6 = temp + i+5;
                    val7 = temp + i+6;
                    val8 = temp + i+7;
                    val9 = temp + i+8;
                    val10 = temp + i+9;
                    val11 = temp + i+10;
                    val12 = temp + i+11;
                    
                    val1->col += horizontal;
                    val2->col += horizontal;
                    val3->col += horizontal;
                    val4->col += horizontal;
                    val5->col += horizontal;
                    val6->col += horizontal;
                    val7->col += horizontal;
                    val8->col += horizontal;
                    val9->col += horizontal;
                    val10->col += horizontal;
                    val11->col += horizontal;
                    val12->col += horizontal;

                    // pixel_with_color[i].col += horizontal;
                }
            }
            rot_transform %= 4;
            if(rot_transform!=0)
            {
                int rot_cnt = rot_transform > 0 ? rot_transform : -(rot_transform*3);
                register int r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12;
                register int c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12;
                for(int i=0; i<rot_cnt; ++i)
                {
                    for(int j=0; j<color_pixel_cnt; j+=12)
                    {
                        val1 = temp + j;
                        val2 = temp + j+1;
                        val3 = temp + j+2;
                        val4 = temp + j+3;
                        val5 = temp + j+4;
                        val6 = temp + j+5;
                        val7 = temp + j+6;
                        val8 = temp + j+7;
                        val9 = temp + j+8;
                        val10 = temp + j+9;
                        val11 = temp + j+10;
                        val12 = temp + j+11;
                        
                        
                        
                        r1 = val1->row; c1 = val1->col;
                        r2 = val2->row; c2 = val2->col;
                        r3 = val3->row; c3 = val3->col;
                        r4 = val4->row; c4 = val4->col;
                        r5 = val5->row; c5 = val5->col;
                        r6 = val6->row; c6 = val6->col;
                        r7 = val7->row; c7 = val7->col;
                        r8 = val8->row; c8 = val8->col;
                        r9 = val9->row; c9 = val9->col;
                        r10 = val10->row; c10 = val10->col;
                        r11 = val11->row; c11 = val11->col;
                        r12 = val12->row; c12 = val12->col;

                        val1->col = render_col - r1; val1->row = render_row + c1;
                        val2->col = render_col - r2; val2->row = render_row + c2;
                        val3->col = render_col - r3; val3->row = render_row + c3;
                        val4->col = render_col - r4; val4->row = render_row + c4;
                        val5->col = render_col - r5; val5->row = render_row + c5;
                        val6->col = render_col - r6; val6->row = render_row + c6;
                        val7->col = render_col - r7; val7->row = render_row + c7;
                        val8->col = render_col - r8; val8->row = render_row + c8;
                        val9->col = render_col - r9; val9->row = render_row + c9;
                        val10->col = render_col - r10; val10->row = render_row + c10;
                        val11->col = render_col - r11; val11->row = render_row + c11;
                        val12->col = render_col - r12; val12->row = render_row + c12;


                        
                        // int row = pixel_with_color[j].row;
                        // int col = pixel_with_color[j].col;
                        // pixel_with_color[j].col = render_col - row;
                        // pixel_with_color[j].row = render_row + col;
                    }
                }
            
            }
            
            if(xMirrored)
            {
                for(int i=0; i<color_pixel_cnt; i+=12)
                {
                    val1 = temp + i;
                    val2 = temp + i+1;
                    val3 = temp + i+2;
                    val4 = temp + i+3;
                    val5 = temp + i+4;
                    val6 = temp + i+5;
                    val7 = temp + i+6;
                    val8 = temp + i+7;
                    val9 = temp + i+8;
                    val10 = temp + i+9;
                    val11 = temp + i+10;
                    val12 = temp + i+11;
                    val1->row = height-1 - val1->row;
                    val2->row = height-1 - val2->row;
                    val3->row = height-1 - val3->row;
                    val4->row = height-1 - val4->row;
                    val5->row = height-1 - val5->row;
                    val6->row = height-1 - val6->row;
                    val7->row = height-1 - val7->row;
                    val8->row = height-1 - val8->row;
                    val9->row = height-1 - val9->row;
                    val10->row = height-1 - val10->row;
                    val11->row = height-1 - val11->row;
                    val12->row = height-1 - val12->row;

                    // pixel_with_color[i].row = (height-1 - pixel_with_color[i].row);
                }
            }

            if(yMirrored)
            {
                for(int i=0; i<color_pixel_cnt; i+=12)
                {
                    val1 = temp + i;
                    val2 = temp + i+1;
                    val3 = temp + i+2;
                    val4 = temp + i+3;
                    val5 = temp + i+4;
                    val6 = temp + i+5;
                    val7 = temp + i+6;
                    val8 = temp + i+7;
                    val9 = temp + i+8;
                    val10 = temp + i+9;
                    val11 = temp + i+10;
                    val12 = temp + i+11;
                    val1->col = height-1 - val1->col;
                    val2->col = height-1 - val2->col;
                    val3->col = height-1 - val3->col;
                    val4->col = height-1 - val4->col;
                    val5->col = height-1 - val5->col;
                    val6->col = height-1 - val6->col;
                    val7->col = height-1 - val7->col;
                    val8->col = height-1 - val8->col;
                    val9->col = height-1 - val9->col;
                    val10->col = height-1 - val10->col;
                    val11->col = height-1 - val11->col;
                    val12->col = height-1 - val12->col;
                    // pixel_with_color[i].col = (width-1 - pixel_with_color[i].col);
                }
            }

            register int idx1, idx2, idx3, idx4, idx5, idx6, idx7, idx8, idx9;
            for(int i=0; i<color_pixel_cnt; ++i)
            {
                val1 = temp + i;
                // val2 = temp + i + 1;
                // val3 = temp + i + 2;
                // val4 = temp + i + 3;
                // val5 = temp + i + 4;
                // val6 = temp + i + 5;
                // val7 = temp + i + 6;
                // val8 = temp + i + 7;
                // val9 = temp + i + 8;

                idx1 = (val1->row * width + val1->col) * 3;
                // idx2 = (val2->row * width + val2->col) * 3;
                // idx3 = (val3->row * width + val3->col) * 3;
                // idx4 = (val4->row * width + val4->col) * 3;
                // idx5 = (val5->row * width + val5->col) * 3;
                // idx6 = (val6->row * width + val6->col) * 3;
                // idx7 = (val7->row * width + val7->col) * 3;
                // idx8 = (val8->row * width + val8->col) * 3;
                // idx9 = (val9->row * width + val9->col) * 3;

                aa = frame + idx1;
                // bb = frame + idx2;
                // cc = frame + idx3;
                // dd = frame + idx4;
                // ee = frame + idx5;
                // ff = frame + idx6;
                // gg = frame + idx7;
                // hh = frame + idx8;
                // ii = frame + idx9;

                memmove(aa, val1->rgb, sizeof(val1->rgb));
                // memmove(bb, val2->rgb, sizeof(val2->rgb));
                // memcpy(cc, val3->rgb, sizeof(val3->rgb));
                // memcpy(dd, val4->rgb, sizeof(val4->rgb));
                // memcpy(ee, val5->rgb, sizeof(val5->rgb));
                // memcpy(ff, val6->rgb, sizeof(val6->rgb));
                // memcpy(gg, val7->rgb, sizeof(val7->rgb));
                // memcpy(hh, val8->rgb, sizeof(val8->rgb));
                // memcpy(ii, val9->rgb, sizeof(val9->rgb)); 

               
                // int idx = (pixel_with_color[i].row * width + pixel_with_color[i].col)*3;
                // frame_buffer[idx] = pixel_with_color[i].rgb[0];
                // frame_buffer[idx+1] = pixel_with_color[i].rgb[1];
                // frame_buffer[idx+2] = pixel_with_color[i].rgb[2];
            }
            
            verifyFrame(frame_buffer, width, height, grading_mode);
            for(int i = 0; i < 4; i++){
		    	dir_transform[i] = 0;
		    }
            // memset(dir_transform, 0, sizeof(dir_transform));
            rot_transform = 0;
            xMirrored = false;
            yMirrored = false;

        }

    }
    return;
}
