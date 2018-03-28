/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#include "fatfs.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */

/* USER CODE BEGIN Variables */

FRESULT f_res;                    
UINT fnum;            					  
BYTE ReadBuffer[1024] = { 0 };       
BYTE WriteBuffer[] = "1234567890abcdefghigk\n";
static void printf_fatfs_error(FRESULT fresult);

/* USER CODE END Variables */    

void MX_FATFS_Init(void) 
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */     
  if (retSD == 0) {

	  f_res = f_mount(&SDFatFS, (TCHAR const*)SDPath, 1);
	  printf_fatfs_error(f_res);
	  if (f_res == FR_OK) {
		  printf("》文件系统挂载成功,可以进行读写测试\n");
		  
		  printf("******文件写入测试... ******\n");
		  f_res = f_open(&SDFile, "FatFs读写测试文件.txt", FA_CREATE_ALWAYS | FA_WRITE);
		  if (f_res == FR_OK)
		  {
			  printf("打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据\n");

			  f_res = f_write(&SDFile, WriteBuffer, sizeof(WriteBuffer), &fnum);
			  if (f_res == FR_OK)
			  {
				  printf("》文件写入成功，写入字节数据： %d\n", fnum);
				  printf("》向文件写入的数据为： \n%s\n", WriteBuffer);
			  }
			  else
			  {
				  printf("！！文件写入失败(%d)\n", f_res);
			  }
			  f_close(&SDFile);
		  }
		  else
		  {
			  printf("！！打开创建文件失败\n");
		  }


		  printf("****** 即将进行文件读测试... ******\n");
		  f_res = f_open(&SDFile, "FatFs读写测试文件.txt", FA_OPEN_EXISTING | FA_READ);
		  if (f_res == FR_OK)
		  {
			  printf("》打开文件成功\n");
			  f_res = f_read(&SDFile, ReadBuffer, sizeof(ReadBuffer), &fnum);
			  if (f_res == FR_OK)
			  {
				  printf("》文件读取成功,读到字节数据： %d\n", fnum);
				  printf("》读取文件数据为： \n%s \n", ReadBuffer);
				  for (size_t i = 0; i < sizeof(ReadBuffer); i++)
				  {
					  printf("%x  ", ReadBuffer[i]);
				  }
			  }
			  else
			  {
				  printf("！！请问年间读取失败： (%d)\n", f_res);
			  }
		  }
		  else
		  {
			  printf("！！文件打开失败。\n");
		  }

		  f_close(&SDFile);
		  f_res = f_mount(NULL, (TCHAR const*)SDPath, 1);
	  }
	  FATFS_UnLinkDriver(SDPath);
  }
  /* USER CODE END Init */
}

/* USER CODE BEGIN Application */
static void printf_fatfs_error(FRESULT fresult)
{
	switch (fresult)
	{
	case FR_OK:                   //(0)
		printf("》操作成功\n");
		break;
	case FR_DISK_ERR:             //(1)
		printf("！！硬件输入输出驱动出错\n");
		break;
	case FR_INT_ERR:              //(2)
		printf("！！断言错误\n");
		break;
	case FR_NOT_READY:            //(3)
		printf("！！物理设备无法工作\n");
		break;
	case FR_NO_FILE:              //(4)
		printf("！！无法找到文件\n");
		break;
	case FR_NO_PATH:              //(5)
		printf("！！无法找到路径\n");
		break;
	case FR_INVALID_NAME:         //(6)
		printf("！！无效的路径名\n");
		break;
	case FR_DENIED:               //(7)
	case FR_EXIST:                //(8)
		printf("！！拒绝访问\n");
		break;
	case FR_INVALID_OBJECT:       //(9)
		printf("！！无效的文件或路径\n");
		break;
	case FR_WRITE_PROTECTED:      //(10)
		printf("！！逻辑设备写保护\n");
		break;
	case FR_INVALID_DRIVE:        //(11)
		printf("！！无效的逻辑设备\n");
		break;
	case FR_NOT_ENABLED:          //(12)
		printf("！！无效的工作区\n");
		break;
	case FR_NO_FILESYSTEM:        //(13)
		printf("！！无效的文件系统\n");
		break;
	case FR_MKFS_ABORTED:         //(14)
		printf("！！因函数参数问题导致f_mkfs函数操作失败\n");
		break;
	case FR_TIMEOUT:              //(15)
		printf("！！操作超时\n");
		break;
	case FR_LOCKED:               //(16)
		printf("！！文件被保护\n");
		break;
	case FR_NOT_ENOUGH_CORE:      //(17)
		printf("！！长文件名支持获取堆空间失败\n");
		break;
	case FR_TOO_MANY_OPEN_FILES:  //(18)
		printf("！！打开太多文件\n");
		break;
	case FR_INVALID_PARAMETER:    // (19)
		printf("！！参数无效\n");
		break;
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
