
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"

/* USER CODE BEGIN Includes */
#include "string.h"
#include "sha-256.h"
#define ECB 1
#define CBC 0
#define CTR 0
#define AES256 1
#include "aes.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

#define ADDR_FLASH_PAGE_31    ((uint32_t)0x08007C00) // Base @ of Page 31, 1 Kbytes
#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_31   // Start @ of user Flash area
#define FLASH_USER_END_ADDR     ADDR_FLASH_PAGE_31 + FLASH_PAGE_SIZE   // End @ of user Flash area

#define BOOTLOADERADR 0x1FFFEC00

#define GH_VERSION "3.00"

#define SECURE_IMPLEM
//#define INSANE

#ifdef SECURE_IMPLEM
uint32_t passWasOK=0;
#endif

typedef struct
{
	uint8_t guard[8];
	uint8_t passwdSHA2[32];
	uint16_t lenSecret;
	uint8_t secretAES[512];
	uint16_t padding;
} dataFlash_t;
#define guardBytes "GHSECRET"

dataFlash_t dataFlashMem;
dataFlash_t *dataFlashPtr=(dataFlash_t *)FLASH_USER_START_ADDR;

struct AES_ctx ctx;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

}

void serPut(uint8_t serByte)
{
	HAL_UART_Transmit(&huart1,&serByte,1,100);
}

//read len char max, keep len-1 (last byte always \0);
// remove the trailing \x0D char
int16_t serReceiveStar(uint8_t * buffer, uint16_t len)
{
	int16_t pos=0;
	for ( ; pos<len ; pos++)
	{
		HAL_UART_Receive(&huart1, buffer+pos, 1, 10000000);
		if (buffer[pos]=='\x0D')
				break;
		serPut('*');
	}
	if (buffer[pos-1]=='\x0D')	// optimisation
		buffer[pos-1]=0;
	buffer[len-1]=0;
	return(pos);
}

//read len char max, keep len-1 (last byte always \0);
// remove the trailing \x0D char
int16_t serReceive(uint8_t * buffer, uint16_t len)
{
	int16_t pos=0;
	for ( ; pos<len ; pos++)
	{
		HAL_UART_Receive(&huart1, buffer+pos, 1, 10000000);
		if (buffer[pos]=='\x0D')
				break;
		serPut(buffer[pos]);
	}
	if (buffer[pos-1]=='\x0D')	// optimisation
		buffer[pos-1]=0;
	buffer[len-1]=0;
	return(pos);
}

int8_t serGet()
{
	uint8_t serByte;
	HAL_StatusTypeDef retVal;
	while(1)
	{
		retVal = HAL_UART_Receive(&huart1,&serByte,1,200);
		if (retVal == HAL_OK)
			return serByte;
	}
}

void serSend(char *serString)
{
	HAL_UART_Transmit(&huart1,(uint8_t*)serString,strlen((char*)serString),100000);
}

#ifdef SECURE_IMPLEM
void checkSecureImplem()
{
	if (passWasOK != 0xcafec0ca)
	{
		memset(&dataFlashMem,0,sizeof(dataFlash_t));
		while(1)
		{
			serSend("\r\n\nUnauthenticate function call, enter in dead loop");
			HAL_Delay(1000);
		}
	}
}
#endif

void displayBoot()
{
	char bootLogo[]= \
			"\x0C\r\n\n" \
			"    ################################\r\n" \
			"    #### GreHack Secret Keeper #####\r\n" \
			"    ################################\r\n" \
			" \r\n" \
			"                .==.\r\n" \
			"              _/____\\_\r\n" \
			"       _.,--'\" ||^ || \"`z._\r\n" \
			"      /_/^ ___\\||  || _/o\\ \"`-._\r\n" \
			"    _/  ]. L_| || .||  \\_/_  . _`--._\r\n" \
			"   /_~7  _ . \" ||. || /] \\ ]. (_)  . \"`--.\r\n" \
			"  |__7~.(_)_ []|+--+|/____T_____________L|\r\n" \
			"  |__|  _^(_) /^   __\\____ _   _|\r\n" \
			"  |__| (_){_) J ]K{__ L___ _   _]\r\n" \
			"  |__| . _(_) \\v     /__________|________\r\n" \
			"  l__l_ (_). []|+-+-<\\^   L  . _   - ---L|\r\n" \
			"   \\__\\    __. ||^l  \\Y] /_]  (_) .  _,--'\r\n" \
			"     \\~_]  L_| || .\\ .\\\\/~.    _,--'\"\r\n" \
			"      \\_\\ . __/||  |\\  \\`-+-<'\"\r\n" \
			"        \"`---._|J__L|X o~~|[\\\\    \"Keep your secrets in a proven ship\"\r\n" \
			"               \\____/ \\___|[//\r\n" \
			"                `--'   `--+-'\r\n" \
			" \r\n";

	serSend(bootLogo);
}

//char decodeFlagInRam_flagLVL2[] =  "GH18{DumpRAMMatterForExploiting}";
char decodeFlagInRam_flagLVL2[] =  {0x29,0xDC,0xA1,0x2C,0xED,0xEF,0x11,0xA1,0xDA,0x35,0x7D,0x80,0xA2,0x2B,0xB2,0x82, \
		                            0x7F,0x17,0x7C,0xE8,0x57,0x35,0xF5,0xFF,0xF5,0x44,0x46,0xDC,0x01,0xD4,0xC3,0xC5};
void decodeFlagInRam()
{
	  const uint8_t flagLVL1[] = "GH18{ST-LINKorBOOTLOADERdumpALL}";
	  AES_init_ctx(&ctx, flagLVL1);	// Flag level1
	  AES_ECB_decrypt(&ctx, (uint8_t*)decodeFlagInRam_flagLVL2);
	  AES_ECB_decrypt(&ctx, (uint8_t*)decodeFlagInRam_flagLVL2+16);
	  memset(&ctx,0,sizeof(struct AES_ctx));
}

int initialized()
{
	//memcpy(&dataFlashMem,(void*)dataFlashPtr,sizeof(dataFlash_t));
	if(memcmp(dataFlashPtr->guard,guardBytes,8))
		return(0);
	return(1);
}

void callBootloader()
{
//	#define BOOTLOADERADR 0x1FFFEC00
	void (*jump) (void);

	HAL_UART_DeInit(&huart1);	// Free UART, MANDATORY !!!!!!!!!!

	HAL_RCC_DeInit();	// RC_DeINit();

	SysTick->CTRL=0;
	SysTick->LOAD=0;
	SysTick->VAL=0;

	__set_PRIMASK(1);
	//__HAL_SYSCFG_REMAPMEMORY_SYSTEMFLASH();
	__set_MSP(*(uint32_t*)BOOTLOADERADR);
	jump=(void (*)(void))(*((uint32_t*)BOOTLOADERADR+1));

	#ifdef SECURE_IMPLEM
	checkSecureImplem();
	#endif

	jump();

}

void backdoor()
{
	int8_t menuItem=0;
	uint8_t *ptr=(uint8_t*)0x08000000;
	char graphity[]= "\r\n" \
			"  ________                ___ ___                __       \r\n" \
			" /  _____/______   ____  /   |   \\_____    ____ |  | __   \r\n" \
			"/   \\  __\\_  __ \\_/ __ \\/    ~    \\__  \\ _/ ___\\|  |/ /   \r\n" \
			"\\    \\_\\  \\  | \\/\\  ___/\\    Y    // __ \\\\  \\___|    <    \r\n" \
			" \\______  /__|    \\___  >\\___|_  /(____  /\\___  >__|_ \\   \r\n" \
			"        \\/            \\/       \\/      \\/     \\/     \\/   \r\n" \
			"__________                __       .___                   \r\n" \
			"\\______   \\_____    ____ |  | __ __| _/____   ___________ \r\n" \
			" |    |  _/\\__  \\ _/ ___\\|  |/ // __ |/  _ \\ /  _ \\_  __ \\\r\n" \
			" |    |   \\ / __ \\\\  \\___|    </ /_/ (  <_> |  <_> )  | \\/\r\n" \
			" |______  /(____  /\\___  >__|_ \\____ |\\____/ \\____/|__|   \r\n" \
			"        \\/      \\/     \\/     \\/    \\/                    \r\n";
	char backdoorMenu[] =	"\r\n" \
							"You entered in management mode, read carefully instructions.\r\n" \
							"-1- Dump flash.\r\n" \
							"-2- Write flash.\r\n" \
				    		"\r\nChoice: ";


	serPut('\x0C');
	serSend(graphity);

	/*	BUGGY API SMT32F030 & read protect kill this way :-(
	serSend(backdoorMenu);
	for (int8_t i = 9 ; i>=0 ; i--)
	{
		serPut(' ');
		serPut('0'+i);
		HAL_Delay(1000);
	}
	serSend("\r\nReboot\r\n");
	callBootloader(); */

	while(1)
	{
		serSend(backdoorMenu);

		switch(serGet())
		{
		case '1':
			serPut('1');
			menuItem=1;
			HAL_Delay(500);
			break;

		case '2':
			serPut('2');
			menuItem=2;
			HAL_Delay(500);
			break;
		} //end menu 1 & 2

		switch(menuItem)
		{
		case 1:
			for (uint32_t i=0 ; i<0x10000 ; i++)
			{
				#ifdef SECURE_IMPLEM
				checkSecureImplem();
				#endif

				serPut(*ptr++);
			}
			while(1)
			{}
			break;

		case 2:
			serSend("\r\n\nNot implemented yet (and I said to not write in flash, to not break the game!)\r\n");
			HAL_Delay(4000);
			break;

		}
		menuItem=0;
		// out of the menu 1 & 2
	}

}

void checkBackdoor()
{
	uint16_t count;
	uint8_t SHA256Hash[32];

#ifndef SECURE_IMPLEM
	char goodPass[32]= \
	{0x9E,0xE6,0xC0,0xFA,0xCB,0xE8,0xD0,0x6E,0x39,0xDA,0x87,0xFF,0x43,0xBF,0x3A,0x4C, \
	 0xE5,0x10,0xAF,0xF2,0x01,0x55,0x18,0xE8,0x41,0x94,0xFC,0xDF,0x6B,0xAE,0x17,0xCF};  // rootbeer
#else
	char goodPass[32]= \
	{0xF0,0x42,0xAE,0x1C,0xAE,0xF3,0x72,0x54,0x63,0x37,0x80,0x11,0x1D,0x65,0x44,0xFB, \
	 0x73,0x48,0xF9,0xCD,0x07,0xEA,0xC7,0x7A,0xCE,0xAA,0xF5,0x57,0xAE,0x99,0xE0,0xCD};  // GreHack2018#$
#endif



	HAL_Delay(500);

	for(count=0 ; count < sizeof(dataFlashMem.secretAES) ; count++)
	{
		HAL_UART_Receive(&huart1, &dataFlashMem.secretAES[count], 1, 10000000);
		calc_sha_256(SHA256Hash,dataFlashMem.secretAES,count+1);
		if (!memcmp(SHA256Hash,goodPass,32))
			backdoor();
	}
/*	TOO easy :)
	if (serGet()=='6')
	{
		serPut('6');
		HAL_Delay(500);
		if (serGet()=='6')
		{
			serPut('6');
			HAL_Delay(500);
			backdoor(); // backdoor here for MAJ
		}
	} */


}

void flash(uint32_t *data,uint32_t len)
{
/*
#define ADDR_FLASH_PAGE_31    ((uint32_t)0x08007C00) // Base @ of Page 31, 1 Kbytes
#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_31   // Start @ of user Flash area
#define FLASH_USER_END_ADDR     ADDR_FLASH_PAGE_31 + FLASH_PAGE_SIZE   // End @ of user Flash area
*/

		uint32_t Address = 0, PageError = 0, curLen;
		uint32_t data32 = 0;
		uint32_t *curData;
		static FLASH_EraseInitTypeDef EraseInitStruct;

		/* Unlock the Flash to enable the flash control register access *************/
		HAL_FLASH_Unlock();

		/* Erase the user Flash area
		(area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

		/* Fill EraseInit structure*/
		EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
		EraseInitStruct.PageAddress = FLASH_USER_START_ADDR;
		EraseInitStruct.NbPages = (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE;

		if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
		{
			/*
			  Error occurred while page erase.
			  User can add here some code to deal with this error.
			  PageError will contain the faulty page and then to know the code error on this page,
			  user can call function 'HAL_FLASH_GetError()'
			*/
			while (1)
			{
			  serSend("\r\nFLASH ERROR: Failed to erase flash");
			  HAL_Delay(2000);
			}
		}

		/* Program the user Flash area word by word
		(area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

		Address = FLASH_USER_START_ADDR;
		curData = data;
		data32 = *curData;
		curLen = 0;

		while ( (curLen < len) && (Address < FLASH_USER_END_ADDR) )
		{
			if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, data32) == HAL_OK)
			{
			  Address += 4;
			  curData += 1;
			  data32 = *curData;
			  curLen += 4;
			}
			else
			{
			  /* Error occurred while writing data in Flash memory.
				 User can add here some code to deal with this error */
			  while (1)
			  {
				serSend("\r\nFLASH ERROR: Failed to program flash");
				HAL_Delay(2000);
			  }
			}
		}

		/* Lock the Flash to disable the flash control register access (recommended
		 to protect the FLASH memory against possible unwanted operation) *********/
		HAL_FLASH_Lock();

		// Check if the programmed data is OK
		if (memcmp((void*)FLASH_USER_START_ADDR,data,len))
		{
			while (1)
			{
			  serSend("\r\nERROR FLASH: Bad flash writing");
			  HAL_Delay(2000);
			}
		}
}

void definePassword()
{
	uint8_t buffer1[50],buffer2[50];
	uint16_t passLen1,passLen2;
	void (*jump) (void);

	char definePasswordMenu1[] = "\x0C\r\nEnter a password: ";
	char definePasswordMenu2[] = "\r\nRepeat password:  ";
	char definePasswordError[] = "\r\n\nPassword mismatch or too short, restart";
	char definePasswordOK[] = "\r\n\nPassword set, rebooting";

	while (1)
	{
		serSend(definePasswordMenu1);
		passLen1=serReceiveStar(buffer1,50);
		serSend(definePasswordMenu2);
		passLen2=serReceiveStar(buffer2,50);
		if( passLen1>2 && passLen1==passLen2 && !memcmp(buffer1,buffer2,passLen1))
		{
			break;
		}
		serSend(definePasswordError);
		HAL_Delay(3000);
	}
	serSend(definePasswordOK);
	HAL_Delay(3000);
	memset(&dataFlashMem,0,sizeof(dataFlash_t));
	memcpy(dataFlashMem.guard,guardBytes,8);
	calc_sha_256(dataFlashMem.passwdSHA2, buffer1, passLen1);
	flash((uint32_t*)&dataFlashMem,sizeof(dataFlash_t));
	jump=(void (*)(void))(*((uint32_t*)0x08000000+1));
	jump();

}

void wipeAll()
{
	int16_t pos;
	uint32_t *ptr=(uint32_t*)&dataFlashMem;
	void (*jump) (void);

	char wipeOut[] = "\r\n" \
	" _       ___            ____        __ \r\n" \
	"| |     / (_)___  ___  / __ \\__  __/ /_\r\n" \
	"| | /| / / / __ \\/ _ \\/ / / / / / / __/\r\n" \
	"| |/ |/ / / /_/ /  __/ /_/ / /_/ / /_  \r\n" \
	"|__/|__/_/ .___/\\___/\\____/\\__,_/\\__/  Everything\r\n" \
	"        /_/                            \r\n";

	char wipeoutDone[] =	"\r\n" \
							"Done\r\n" \
							"In 10 seconds the device will reboot\r\n" \
							"\r\n10";

	serPut('\x0C');
	serSend(wipeOut);
	for (pos=0 ; pos < sizeof(dataFlash_t); pos += sizeof(uint32_t) )
		*ptr++=0xfeebdaed;
	flash( (uint32_t*)&dataFlashMem, sizeof(dataFlash_t) );

	serSend(wipeoutDone);
	for (int8_t i = 9 ; i>=0 ; i--)
	{
		serPut(' ');
		serPut('0'+i);
		HAL_Delay(1000);
	}
	serSend("\r\nReboot\r\n");

	jump=(void (*)(void))(*((uint32_t*)0x08000000+1));
	jump();
}

void setSecret()
{
	char setSecretMenu[] = "\x0C\r\nEnter your secret here. Up to 512 bytes can be stored.\r\n\n";
	char setSecretOK[] = "\r\n\nYour secret was stored successfully";
	int16_t i;

	serSend(setSecretMenu);

	// load guard & password in RAM
	memcpy(&dataFlashMem,(void*)dataFlashPtr,sizeof(dataFlash_t));

	memset(dataFlashMem.secretAES,0,sizeof(dataFlashMem.secretAES));
	dataFlashMem.lenSecret=serReceive(dataFlashMem.secretAES,sizeof(dataFlashMem.secretAES));

    AES_init_ctx(&ctx, dataFlashMem.passwdSHA2);
    for(i=0 ; i<sizeof(dataFlashMem.secretAES)/16 ; i++)
    {
    	AES_ECB_encrypt(&ctx, &dataFlashMem.secretAES[i*16]);
    }
	flash((uint32_t*)&dataFlashMem,sizeof(dataFlash_t));

	memset(&dataFlashMem,0,sizeof(dataFlash_t));

	serSend(setSecretOK);
	HAL_Delay(3000);
}

void retreiveSecret()
{
	char retreiveSecretMenu[] = "\x0C\r\nThe secret store in your device is :\r\n\n";
	char setSecretOK[] = "\r\n\nPress ENTER to continue";
	int16_t i;

	serSend(retreiveSecretMenu);

	memcpy(&dataFlashMem,(void*)dataFlashPtr,sizeof(dataFlash_t));

	#ifdef SECURE_IMPLEM
	checkSecureImplem();
	#endif

    AES_init_ctx(&ctx, dataFlashMem.passwdSHA2);
    for(i=0 ; i<sizeof(dataFlashMem.secretAES)/16 ; i++)
    	AES_ECB_decrypt(&ctx, &dataFlashMem.secretAES[i*16]);

    for(i=0 ; i<dataFlashMem.lenSecret ; i++)
    	serPut(dataFlashMem.secretAES[i]);

    memset(dataFlashMem.secretAES,0,sizeof(dataFlashMem.secretAES));

	HAL_Delay(3000);
	serSend(setSecretOK);
	while(serGet()!='\x0D')
	{}

}

void productNFO()
{
	char ghost[]= "\r\n" \
			".......................................\r\n" \
			"..............OOOOo....................\r\n" \
			"............oOOOOOOo...................\r\n" \
			"............oOOOOOOO...................\r\n" \
			"............oOOOOOOOOOOOO..............\r\n" \
			"..........OOOOOOOOOOOOOOOOOOO..........\r\n" \
			"........OOOOOOOOOOOOOOOOOOOOOOO........\r\n" \
			".......OOOOOOOOOOOOOOOOOOOOOOOOOo......\r\n" \
			".....OOOOOOOOOOOOOOOOOOOOOOOOOOOOO.....\r\n" \
			"....OOOOOOOOOOOOOOOOOoOoOo..ooOoOOO....\r\n" \
			"...oOOOOOOOOOO.oOOOOOOOOOOOOOOOOOOOo...\r\n" \
			"...OOOOOO.oOOOOOOOOOOOOOOOOOOOOOOOOOO..\r\n" \
			"..OOO.oOOOOOOOOOOOOo..oO......Oo..OOO..\r\n" \
			"..OOOOOOOOOOO.OOOOOOOOO........OOOOOO..\r\n" \
			".OOOOOOO.......OOOOOOO..........OOOOOO.\r\n" \
			".oOOo.OO.......OOOOOOO..........OOOOOO.\r\n" \
			"...OOOOO........OOOOOO..........OOOOOO.\r\n" \
			"..OOOOOO.......oOOOOOO..........OOOOOo.\r\n" \
			"..oOOOOOo......OOOOOOOO........oOOOOO..\r\n" \
			"...OOOOOOO....OOOOOOOOOO......OOOOOOO..\r\n" \
			"....OOOOOOOOOOOOOOOOOOOOOO..OOOOOOOO...\r\n" \
			"....OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO....\r\n" \
			".....OOOOOOOOOOOOOOOOOOOOOOOOOOOOOO....\r\n" \
			"....OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO...\r\n" \
			"....OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO...\r\n" \
			"....OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO....\r\n" \
			".....OOOOOOOOOOOOOOOOOOOOOOOOOOOOOO....\r\n" \
			"......OOOOOOOo.OOOOOOOOOO..OOOOOOO.....\r\n" \
			"........OOOo....OOOOOOOO...............\r\n" \
			".................oOOOO.................\r\n" \
			".......................................\r\n";
	char NFO[]= \
			"\r\n*******************************************************************************\r\n" \
			"**************************** GreHack Secret Keeper ****************************\r\n" \
			"*******************************************************************************\r\n" \
			"\r\n" \
			"-------------------------------------------------------------------------------\r\n" \
			"                              General Information\r\n" \
			"-------------------------------------------------------------------------------\r\n" \
			"Type.................: Utility\r\n" \
			"Platform.............: STM32\r\n" \
			"\r\n" \
			"-------------------------------------------------------------------------------\r\n" \
			"                                 Release Notes\r\n" \
			"-------------------------------------------------------------------------------\r\n" \
			"Version ";

	serPut('\x0C');
	serSend(ghost);
	serSend(NFO);
	serSend(GH_VERSION);
	serSend("\r\n\nPress ENTER to continue");
	while(serGet()!='\x0D')
	{}
}

char checkPassword_enterPass[] = "\x0C\r\n" \
				"Enter the password: ";
uint8_t checkPassword_pos;
uint8_t checkPassword_passwdSHA2[32];

uint8_t checkPassword()
{
	uint8_t buffer[50];

	checkPassword_pos=0;

	serSend(checkPassword_enterPass);

#ifndef INSANE
	while(1)		// buffer overflow here
#else
	for (uint8_t i=0 ; i < 50 ; i++)
#endif
	{
		HAL_UART_Receive(&huart1, buffer+checkPassword_pos, 1, 10000000);
		if (buffer[checkPassword_pos++]=='\x0D')
				break;
	}
	buffer[checkPassword_pos-1]=0;
	calc_sha_256(checkPassword_passwdSHA2, buffer, strlen((char*)buffer));
	if (memcmp(checkPassword_passwdSHA2,dataFlashPtr->passwdSHA2,32))
		return (0);

	#ifdef SECURE_IMPLEM
	passWasOK = 0xcafec0ca;
	#endif

	return(1);
}

char generatePassword_menu[] = "\x0C\r\n" \
				"Helper function to generate a good password.\r\n\nPlease enter 8 random chars max: ";
char generatePassword_done[] = "\r\n\nGenerated password: ";
uint8_t generatePassword_pos=0;

void generatePassword()
{
	uint8_t buffer[9];

	generatePassword_pos=0;

	serSend(generatePassword_menu);

	#ifdef SECURE_IMPLEM
	for (uint8_t i=0 ; i<8 ; i++)
	#else
	while(1)		// buffer overflow here
	#endif
	{
		HAL_UART_Receive(&huart1, &buffer[generatePassword_pos], 1, 10000000);
		serPut(buffer[generatePassword_pos]);
		if (buffer[generatePassword_pos++]=='\x0D')
				break;
	}
	buffer[checkPassword_pos-1]=0;

	// hash random values
	calc_sha_256(dataFlashMem.passwdSHA2, buffer, checkPassword_pos);
	// keep 4 bytes of hash
	memcpy(buffer+4,dataFlashMem.passwdSHA2,4);
	// add the systick
	*((uint32_t*)buffer)=HAL_GetTick();
	// hash again
	calc_sha_256(dataFlashMem.passwdSHA2, buffer, 8);

	serSend(generatePassword_done);

	for (uint8_t i=0 ; i<12 ; i++)
	{
		if ( !(dataFlashMem.passwdSHA2[i]&0x03) )
			serPut('0'+dataFlashMem.passwdSHA2[i+12]%10);
		else if ( !(dataFlashMem.passwdSHA2[i]&0x04) )
			serPut('A'+dataFlashMem.passwdSHA2[i+12]%26);
		else
			serPut('a'+dataFlashMem.passwdSHA2[i+12]%26);
	}

	serSend("\r\n\nPress ENTER to continue");
	while(serGet()!='\x0D')
	{}

}

void changePassword()
{
	int16_t i;
	char definePasswordMenu1[] = "\x0C\r\nEnter a password: ";
	char definePasswordMenu2[] = "\r\nRepeat password:  ";
	char definePasswordError[] = "\r\nPassword mismatch or too short, restart";
	char definePasswordOK[] = "\r\n\nPassword set";

	uint8_t buffer1[50],buffer2[50];
	uint16_t passLen1,passLen2;

	//decrypt secret in RAM
	memcpy(&dataFlashMem,(void*)dataFlashPtr,sizeof(dataFlash_t));
	#ifdef SECURE_IMPLEM
	checkSecureImplem();
	#endif

    AES_init_ctx(&ctx, dataFlashMem.passwdSHA2);
    for(i=0 ; i<sizeof(dataFlashMem.secretAES)/16 ; i++)
    	AES_ECB_decrypt(&ctx, &dataFlashMem.secretAES[i*16]);

    // get the new pass
	while (1)
	{
		serSend(definePasswordMenu1);
		passLen1=serReceiveStar(buffer1,50);
		serSend(definePasswordMenu2);
		passLen2=serReceiveStar(buffer2,50);
		if( passLen1>2 && passLen1==passLen2 && !memcmp(buffer1,buffer2,passLen1))
		{
			break;
		}
		serSend(definePasswordError);
		HAL_Delay(3000);
	}
	calc_sha_256(dataFlashMem.passwdSHA2, buffer1, passLen1);

    //crypt again the secret
    AES_init_ctx(&ctx, dataFlashMem.passwdSHA2);
    for(i=0 ; i<sizeof(dataFlashMem.secretAES)/16 ; i++)
    	AES_ECB_encrypt(&ctx, &dataFlashMem.secretAES[i*16]);

    flash((uint32_t*)&dataFlashMem,sizeof(dataFlash_t));

    memset(&dataFlashMem,0,sizeof(dataFlashMem));

    serSend(definePasswordOK);
	HAL_Delay(3000);
}

void virginDevice()
{
	  // device virgin, no data
	  int8_t menuItem=0;
	  char liteMenu[] = "\x0C\r\n" \
	  	  	  			"*** Secret Keeper is empty ***\r\n\n" \
			  	  	  	"-1- Set a password.\r\n" \
						"-2- Generate good password.\r\n" \
						"-3- Product information.\r\n" \
			    		"\r\nChoice: ";
		while(1)
		{
			serSend(liteMenu);

			switch(serGet())
			{
			case '1':
				serPut('1');
				menuItem=1;
				HAL_Delay(500);
				break;

			case '2':
				serPut('2');
				menuItem=2;
				HAL_Delay(500);
				break;

			case '3':
				serPut('3');
				menuItem=3;
				HAL_Delay(500);
				break;

			case '6':
				serPut('6');
				checkBackdoor();
				break;

			} //end menu 1 & 2

			switch(menuItem)
			{
			case 1:
				definePassword();
				HAL_Delay(2000);
				break;

			case 2:
				generatePassword();
				break;

			case 3:
				productNFO();
				break;
			}
			menuItem=0;
			// out of the menu 1 & 2
		}

}

void loadedDevice()
{
	  // device contain password and data
	  int8_t menuItem=0;
	  char fullMenuPass[] = "\x0C\r\n" \
			  	  	  	"*** Secret Keeper locked ***\r\n\n" \
			  	  	  	"-1- Enter the password.\r\n" \
						"-2- Lost password, wipe all.\r\n" \
						"-3- Generate good password.\r\n" \
						"-4- Product information.\r\n" \
			    		"\r\nChoice: ";

	  char fullMenu[] = "\x0C\r\n" \
			  	  	  	"*** Secret Keeper unlocked ***\r\n\n" \
			  	  	  	"-1- Change password.\r\n" \
						"-2- Retrieve secret information.\r\n" \
						"-3- Set secret information.\r\n" \
						"-4- Product information.\r\n" \
			    		"\r\nChoice: ";

		while(1)
		{
			serSend(fullMenuPass);

			switch(serGet())
			{
			case '1':
				serPut('1');
				menuItem=1;
				HAL_Delay(500);
				break;

			case '2':
				serPut('2');
				menuItem=2;
				HAL_Delay(500);
				break;

			case '3':
				serPut('3');
				menuItem=3;
				HAL_Delay(500);
				break;

			case '4':
				serPut('4');
				menuItem=4;
				HAL_Delay(500);
				break;

			case '6':
				serPut('6');
				checkBackdoor();
				break;

			} //end menu 1 & 2

			switch(menuItem)
			{
			case 1:
				if (checkPassword())
					goto passIsGood;
				break;

			case 2:
				wipeAll();
				break;

			case 3:
				generatePassword();
				break;

			case 4:
				productNFO();
				break;
			}
			menuItem=0;
		}

	  passIsGood:
	  while(1)
		{
			serSend(fullMenu);
			switch(serGet())
			{
			case '1':
				serPut('1');
				menuItem=1;
				HAL_Delay(500);
				break;

			case '2':
				serPut('2');
				menuItem=2;
				HAL_Delay(500);
				break;

			case '3':
				serPut('3');
				menuItem=3;
				HAL_Delay(500);
				break;

			case '4':
				serPut('4');
				menuItem=4;
				HAL_Delay(500);
				break;

			case '6':
				serPut('6');
				checkBackdoor();
				break;
			}

			switch(menuItem)
			{
			case 1:
				changePassword();
				break;

			case 2:
				retreiveSecret();
				break;

			case 3:
				setSecret();
				break;

			case 4:
				productNFO();
				break;
			}
			menuItem=0;
		}
}

void flagHolder()
{
	char flagLVL3[] = "A";
	uint8_t SHA256Hash[32];

	calc_sha_256(SHA256Hash,flagLVL3,strlen(flagLVL3));
	if (*(uint32_t*)SHA256Hash == 0x7557696e)
		serSend("https://www.youtube.com/watch?v=WPzMxiGd0io This code is a flag holder, impossible to reach. BTW, your flag for level 3 is GH18{TheresAlwAYSAbackDOOR...}.");
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  decodeFlagInRam();
  displayBoot();
  flagHolder();		// keep flag for LVL3 in flash
  serSend("\r\nPress ENTER to start");
  while(serGet()!='\x0D')
  {}

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

	  if (!initialized() )
	  {
		  virginDevice();
	  }
	  else
	  {
		  loadedDevice();
	  }

	  //HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_15);
	  //HAL_Delay(300);

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT;
  huart1.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  huart1.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
