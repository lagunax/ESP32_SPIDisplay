/*
 * Defaults.h
 *
 *  Created on: 14 окт. 2019 г.
 *      Author: producer
 */

#ifndef COMPONENTS_ARDUINO_LIBRARIES_ESP32_SPIDISPLAY_DEFAULTS_H_
	#define COMPONENTS_ARDUINO_LIBRARIES_ESP32_SPIDISPLAY_DEFAULTS_H_

#ifndef ESP32
	#define ESP32 //Just used to test ESP32 options
#endif



		#ifndef TAB_COLOUR
			#define TAB_COLOUR 0
		#endif

		// If the frequency is not defined, set a default
		#ifndef SPI_FREQUENCY
			#warning "SPI_FREQUENCY not defined, using default 20000000"
			#define SPI_FREQUENCY 20000000
		#endif

		// If the frequency is not defined, set a default
		#ifndef SPI_READ_FREQUENCY
			#warning "SPI_READ_FREQUENCY not defined, using SPI_FREQUENCY as default"
			#define SPI_READ_FREQUENCY SPI_FREQUENCY
		#endif

		// If the frequency is not defined, set a default
		#ifndef SPI_TOUCH_FREQUENCY
			#define SPI_TOUCH_FREQUENCY 2500000
		#endif

		#if defined(ST7789_DRIVER) || defined(ST7789_2_DRIVER)
			#define TFT_SPI_MODE SPI_MODE3
		#else
			#define TFT_SPI_MODE SPI_MODE0
		#endif

		#if defined (ESP32)
		  #if !defined (ESP32_PARALLEL)
			#ifdef USE_HSPI_PORT
			  static SPIClass spi = SPIClass(HSPI);
			#else // use default VSPI port
			  static SPIClass& spi = SPI;
			#endif
		  #endif
		#else // ESP8266
		  static SPIClass& spi = SPI;
		#endif


	namespace Device{
/*
		#if defined (ESP32)
		  #if !defined (ESP32_PARALLEL)
			#ifdef USE_HSPI_PORT
			  static SPIClass spi = SPIClass(HSPI);
			#else // use default VSPI port
			  static SPIClass& spi = SPI;
			#endif
		  #endif
		#else // ESP8266
		  static SPIClass& spi = SPI;
		#endif
*/

/*			inline void		spi_begin() __attribute__((always_inline)),
							spi_end()   __attribute__((always_inline)),
							spi_begin_read() __attribute__((always_inline)),
							spi_end_read()   __attribute__((always_inline));

*/

		namespace Display{

		namespace{
			  // SUPPORT_TRANSACTIONS is mandatory for ESP32 so the hal mutex is toggled
			#if defined (ESP32) && !defined (SUPPORT_TRANSACTIONS)
				#define SUPPORT_TRANSACTIONS
			#endif

			// If it is a 16bit serial display we must transfer 16 bits every time
			#ifdef RPI_ILI9486_DRIVER
				#define CMD_BITS 16-1
			#else
				#define CMD_BITS 8-1
			#endif

			#ifndef TFT_DC
				#define DC_C // No macro allocated so it generates no code
				#define DC_D // No macro allocated so it generates no code
			#else
				#if defined (ESP8266) && (TFT_DC == 16)
					#define DC_C digitalWrite(TFT_DC, LOW)
					#define DC_D digitalWrite(TFT_DC, HIGH)
			  	#elif defined (ESP32)
					#if defined (ESP32_PARALLEL)
				  		#define DC_C GPIO.out_w1tc = (1 << TFT_DC)
				  	  	#define DC_D GPIO.out_w1ts = (1 << TFT_DC)
					#else
				  		#if TFT_DC >= 32
							#ifdef RPI_ILI9486_DRIVER  // RPi display needs a slower DC change
					  	  		#define DC_C GPIO.out1_w1ts.val = (1 << (TFT_DC - 32)); \
										GPIO.out1_w1tc.val = (1 << (TFT_DC - 32))
					  	  	  	#define DC_D GPIO.out1_w1tc.val = (1 << (TFT_DC - 32)); \
										GPIO.out1_w1ts.val = (1 << (TFT_DC - 32))
							#else
					  	  		#define DC_C GPIO.out1_w1tc.val = (1 << (TFT_DC - 32))//;GPIO.out1_w1tc.val = (1 << (TFT_DC - 32))
					  	  	  	#define DC_D GPIO.out1_w1ts.val = (1 << (TFT_DC - 32))//;GPIO.out1_w1ts.val = (1 << (TFT_DC - 32))
							#endif
				  	  	#else
							#if TFT_DC >= 0
					  	  		#ifdef RPI_ILI9486_DRIVER  // RPi display needs a slower DC change
									#define DC_C GPIO.out_w1ts = (1 << TFT_DC); \
											GPIO.out_w1tc = (1 << TFT_DC)
									#define DC_D GPIO.out_w1tc = (1 << TFT_DC); \
									 	 	GPIO.out_w1ts = (1 << TFT_DC)
					  	  	  	#else
									#define DC_C GPIO.out_w1tc = (1 << TFT_DC)//;GPIO.out_w1tc = (1 << TFT_DC)
									#define DC_D GPIO.out_w1ts = (1 << TFT_DC)//;GPIO.out_w1ts = (1 << TFT_DC)
					  	  	  	#endif
							#else
					  	  	  	#define DC_C
					  	  	  	#define DC_D
							#endif
				  	  	#endif
					#endif
				#else
					#define DC_C GPOC=dcpinmask
					#define DC_D GPOS=dcpinmask
				#endif
			#endif

			#if defined (TFT_SPI_OVERLAP)
			  #undef TFT_CS
			  #define SPI1U_WRITE (SPIUMOSI | SPIUSSE | SPIUCSSETUP | SPIUCSHOLD)
			  #define SPI1U_READ  (SPIUMOSI | SPIUSSE | SPIUCSSETUP | SPIUCSHOLD | SPIUDUPLEX)
			#else
			  #ifdef ESP8266
				#define SPI1U_WRITE (SPIUMOSI | SPIUSSE)
				#define SPI1U_READ  (SPIUMOSI | SPIUSSE | SPIUDUPLEX)
			  #endif
			#endif

			#ifndef TFT_CS
			  #define CS_L // No macro allocated so it generates no code
			  #define CS_H // No macro allocated so it generates no code
			#else
			  #if defined (ESP8266) && (TFT_CS == 16)
				#define CS_L digitalWrite(TFT_CS, LOW)
				#define CS_H digitalWrite(TFT_CS, HIGH)
			  #elif defined (ESP32)
				#if defined (ESP32_PARALLEL)
				  #define CS_L // The TFT CS is set permanently low during init()
				  #define CS_H
				#else
				  #if TFT_CS >= 32
					#ifdef RPI_ILI9486_DRIVER  // RPi display needs a slower CS change
					  #define CS_L GPIO.out1_w1ts.val = (1 << (TFT_CS - 32)); \
								   GPIO.out1_w1tc.val = (1 << (TFT_CS - 32))
					  #define CS_H GPIO.out1_w1tc.val = (1 << (TFT_CS - 32)); \
								   GPIO.out1_w1ts.val = (1 << (TFT_CS - 32))
					#else
					  #define CS_L GPIO.out1_w1tc.val = (1 << (TFT_CS - 32)); GPIO.out1_w1tc.val = (1 << (TFT_CS - 32))
					  #define CS_H GPIO.out1_w1ts.val = (1 << (TFT_CS - 32))//;GPIO.out1_w1ts.val = (1 << (TFT_CS - 32))
					#endif
				  #else
					#if TFT_CS >= 0
					  #ifdef RPI_ILI9486_DRIVER  // RPi display needs a slower CS change
						#define CS_L GPIO.out_w1ts = (1 << TFT_CS); GPIO.out_w1tc = (1 << TFT_CS)
						#define CS_H GPIO.out_w1tc = (1 << TFT_CS); GPIO.out_w1ts = (1 << TFT_CS)
					  #else
						#define CS_L GPIO.out_w1tc = (1 << TFT_CS);GPIO.out_w1tc = (1 << TFT_CS)
						#define CS_H GPIO.out_w1ts = (1 << TFT_CS)//;GPIO.out_w1ts = (1 << TFT_CS)
					  #endif
					#else
					  #define CS_L
					  #define CS_H
					#endif
				  #endif
				#endif
			  #else
				#define CS_L GPOC=cspinmask
				#define CS_H GPOS=cspinmask
			  #endif
			#endif

			// Use single register write for CS_L and DC_C if pins are both in range 0-31
			#ifdef ESP32
			  #ifdef TFT_CS
				#if (TFT_CS >= 0) && (TFT_CS < 32) && (TFT_DC >= 0) && (TFT_DC < 32)
				  #ifdef RPI_ILI9486_DRIVER  // RPi display needs a slower CD and DC change
					#define CS_L_DC_C GPIO.out_w1tc = ((1 << TFT_CS) | (1 << TFT_DC)); \
									  GPIO.out_w1tc = ((1 << TFT_CS) | (1 << TFT_DC))
				  #else
					#define CS_L_DC_C GPIO.out_w1tc = ((1 << TFT_CS) | (1 << TFT_DC)); GPIO.out_w1tc = ((1 << TFT_CS) | (1 << TFT_DC))
				  #endif
				#else
				  #define CS_L_DC_C CS_L; DC_C
				#endif
			  #else
				#define CS_L_DC_C CS_L; DC_C
			  #endif
			#else // ESP8266
			  #define CS_L_DC_C CS_L; DC_C
			#endif

			// chip select signal for touchscreen
			#ifndef TOUCH_CS
			  #define T_CS_L // No macro allocated so it generates no code
			  #define T_CS_H // No macro allocated so it generates no code
			#else
			  #define T_CS_L digitalWrite(TOUCH_CS, LOW)
			  #define T_CS_H digitalWrite(TOUCH_CS, HIGH)
			#endif


			#ifdef TFT_WR
			  #if defined (ESP32)
				#define WR_L GPIO.out_w1tc = (1 << TFT_WR)
				#define WR_H GPIO.out_w1ts = (1 << TFT_WR)
			  #else
				#define WR_L GPOC=wrpinmask
				#define WR_H GPOS=wrpinmask
			  #endif
			#endif

			#ifdef ESP8266
			  // Concatenate two 16 bit values for the SPI 32 bit register write
			  #define SPI_32(H,L) ( (H)<<16 | (L) )
			  #define COL_32(H,L) ( (H)<<16 | (L) )
			#else
			  #if defined (ESP32_PARALLEL) || defined (ILI9488_DRIVER)
				#define SPI_32(H,L) ( (H)<<16 | (L) )
			  #else
				#define SPI_32(H,L) ( ((H)<<8 | (H)>>8) | (((L)<<8 | (L)>>8)<<16 ) )
			  #endif
			  // Swap byte order for concatenated 16 bit colors
			  // AB CD -> DCBA for 32 bit register write
			  #define COL_32(H,L) ( ((H)<<8 | (H)>>8) | (((L)<<8 | (L)>>8)<<16 ) )
			#endif

			#if defined (ESP32) && defined (ESP32_PARALLEL)
			  // Mask for the 8 data bits to set pin directions
			  #define dir_mask ((1 << TFT_D0) | (1 << TFT_D1) | (1 << TFT_D2) | (1 << TFT_D3) | (1 << TFT_D4) | (1 << TFT_D5) | (1 << TFT_D6) | (1 << TFT_D7))

			  // Data bits and the write line are cleared to 0 in one step
			  #define clr_mask (dir_mask | (1 << TFT_WR))

			  // A lookup table is used to set the different bit patterns, this uses 1kByte of RAM
			  #define set_mask(C) xset_mask[C] // 63fps Sprite rendering test 33% faster, graphicstest only 1.8% faster than shifting in real time

			  // Real-time shifting alternative to above to save 1KByte RAM, 47 fps Sprite rendering test
			  /*#define set_mask(C) ((C&0x80)>>7)<<TFT_D7 | ((C&0x40)>>6)<<TFT_D6 | ((C&0x20)>>5)<<TFT_D5 | ((C&0x10)>>4)<<TFT_D4 | \
									((C&0x08)>>3)<<TFT_D3 | ((C&0x04)>>2)<<TFT_D2 | ((C&0x02)>>1)<<TFT_D1 | ((C&0x01)>>0)<<TFT_D0
			  //*/

			  // Write 8 bits to TFT
			  #define tft_Write_8(C)  GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t)C); WR_H

			  // Write 16 bits to TFT
			  #ifdef PSEUDO_8_BIT
				#define tft_Write_16(C) WR_L;GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t)(C >> 0)); WR_H
			  #else
				#define tft_Write_16(C) GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t)(C >> 8)); WR_H; \
										GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t)(C >> 0)); WR_H
			  #endif

			  // 16 bit write with swapped bytes
			  #define tft_Write_16S(C) GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) (C >>  0)); WR_H; \
									   GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) (C >>  8)); WR_H

			  // Write 32 bits to TFT
			  #define tft_Write_32(C) GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) (C >> 24)); WR_H; \
									  GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) (C >> 16)); WR_H; \
									  GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) (C >>  8)); WR_H; \
									  GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) (C >>  0)); WR_H

			  #ifdef TFT_RD
				#define RD_L GPIO.out_w1tc = (1 << TFT_RD)
				//#define RD_L digitalWrite(TFT_WR, LOW)
				#define RD_H GPIO.out_w1ts = (1 << TFT_RD)
				//#define RD_H digitalWrite(TFT_WR, HIGH)
			  #endif

			#elif  defined (ILI9488_DRIVER) // 16 bit color converted to 3 bytes for 18 bit RGB

			  // Write 8 bits to TFT
			  #define tft_Write_8(C)   spi.transfer(C)

			  // Convert 16 bit color to 18 bit and write in 3 bytes
			  #define tft_Write_16(C)  spi.transfer((C & 0xF800)>>8); \
									   spi.transfer((C & 0x07E0)>>3); \
									   spi.transfer((C & 0x001F)<<3)

			  // Convert swapped byte 16 bit color to 18 bit and write in 3 bytes
			  #define tft_Write_16S(C) spi.transfer(C & 0xF8); \
									   spi.transfer((C & 0xE000)>>11 | (C & 0x07)<<5); \
									   spi.transfer((C & 0x1F00)>>5)
			  // Write 32 bits to TFT
			  #define tft_Write_32(C)  spi.write32(C)

			#elif  defined (RPI_ILI9486_DRIVER)

			  #define tft_Write_8(C)   spi.transfer(0); spi.transfer(C)
			  #define tft_Write_16(C)  spi.write16(C)
			  #define tft_Write_16S(C) spi.write16(C<<8 | C>>8)
			  #define tft_Write_32(C)  spi.write32(C)

			#elif defined ESP8266

			  #define tft_Write_8(C)   spi.write(C)
			  #define tft_Write_16(C)  spi.write16(C)
			  #define tft_Write_32(C)  spi.write32(C)

			#else // ESP32 using SPI with 16 bit color display

			  // ESP32 low level SPI writes for 8, 16 and 32 bit values
			  // to avoid the function call overhead

			  // Write 8 bits
			  #define tft_Write_8(C) \
			  WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), 8-1); \
			  WRITE_PERI_REG(SPI_W0_REG(SPI_PORT), C); \
			  SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR); \
			  while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);

			  // Write 16 bits with corrected endianess for 16 bit colors
			  #define tft_Write_16(C) \
			  WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), 16-1); \
			  WRITE_PERI_REG(SPI_W0_REG(SPI_PORT), C<<8 | C>>8); \
			  SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR); \
			  while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);

			  // Write 16 bits
			  #define tft_Write_16S(C) \
			  WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), 16-1); \
			  WRITE_PERI_REG(SPI_W0_REG(SPI_PORT), C); \
			  SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR); \
			  while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);

			  // Write 32 bits
			  #define tft_Write_32(C) \
			  WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), 32-1); \
			  WRITE_PERI_REG(SPI_W0_REG(SPI_PORT), C); \
			  SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR); \
			  while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);

			#endif


			#if !defined (ESP32_PARALLEL)

			  // Read from display using SPI or software SPI
			  #if defined (ESP8266) && defined (TFT_SDA_READ)
				// Use a bit banged function call for ESP8266 and bi-directional SDA pin
				#define SCLK_L GPOC=sclkpinmask
				#define SCLK_H GPOS=sclkpinmask
			  #else
				// Use a SPI read transfer
				#define tft_Read_8() spi.transfer(0)
			  #endif

			  // Make sure TFT_MISO is defined if not used to avoid an error message
			  #ifndef TFT_MISO
				#define TFT_MISO -1
			  #endif

			#endif



			//These enumerate the text plotting alignment (reference datum point)
			#define TL_DATUM 0 // Top left (default)
			#define TC_DATUM 1 // Top center
			#define TR_DATUM 2 // Top right
			#define ML_DATUM 3 // Middle left
			#define CL_DATUM 3 // Center left, same as above
			#define MC_DATUM 4 // Middle center
			#define CC_DATUM 4 // Center center, same as above
			#define MR_DATUM 5 // Middle right
			#define CR_DATUM 5 // Center right, same as above
			#define BL_DATUM 6 // Bottom left
			#define BC_DATUM 7 // Bottom center
			#define BR_DATUM 8 // Bottom right
			#define L_BASELINE  9 // Left character baseline (Line the 'A' character would sit on)
			#define C_BASELINE 10 // Center character baseline
			#define R_BASELINE 11 // Right character baseline


			// New color definitions use for libraries
			#define TFT_BLACK       0x0000      /*   0,   0,   0 */
			#define TFT_NAVY        0x000F      /*   0,   0, 128 */
			#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
			#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
			#define TFT_MAROON      0x7800      /* 128,   0,   0 */
			#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
			#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
			#define TFT_LIGHTGREY   0xC618      /* 192, 192, 192 */
			#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
			#define TFT_BLUE        0x001F      /*   0,   0, 255 */
			#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
			#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
			#define TFT_RED         0xF800      /* 255,   0,   0 */
			#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
			#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
			#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
			#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
			#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
			#define TFT_PINK        0xFC9F

			// Next is a special 16 bit color value that encodes to 8 bits
			// and will then decode back to the same 16 bit value.
			// Convenient for 8 bit and 16 bit transparent sprites.
			#define TFT_TRANSPARENT 0x0120

			// Swap any type
			template <typename T> static inline void
			swap_coord(T& a, T& b) { T t = a; a = b; b = t; }

			#ifndef min
			  // Return minimum of two numbers, may already be defined
			  #define min(a,b) (((a) < (b)) ? (a) : (b))
			#endif
		}
		}
	}

#endif /* COMPONENTS_ARDUINO_LIBRARIES_TFT_ESPI_DEFAULTS_H_ */
