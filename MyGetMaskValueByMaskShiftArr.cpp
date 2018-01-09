/***********************************************************\
 利用 Ptr查表法, 快速取出 Mask 位置上的 ImagePixelValue 方法
 可使用 compileonline 測試
 http://www.compileonline.com/compile_cpp_online.php
 Last Update : 20180109 by smallpig01
 
 Main::ImageWH 設定影像資料寬高
 Main::MaskWH  設定Mask寬高
 Main::MaskCenterPosX/Y 設定 Mask 中心座標
\***********************************************************/

#include <iostream>
using namespace std;
typedef unsigned char BYTE; //替標準C++建立BYTE

int PrintMaskValue(int Mask[], int MaskWH)
{   //印出陣列資料
    int ArrLength = MaskWH*MaskWH;
    int Temp=0;
    for(int i=0;i<ArrLength;i++)
    {
        //Temp++;
		printf("%4d,", Mask[i]);
		if(++Temp%MaskWH==0) 
		{
		    printf("\n");
		}
	}    
}

int PrintMaskValue(BYTE Mask[], int MaskWH)
{   //印出陣列資料
    int ArrLength = MaskWH*MaskWH;
    int Temp=0;
    for(int i=0;i<ArrLength;i++)
    {
        //Temp++;
		printf("%4d,", Mask[i]);
		if(++Temp%MaskWH==0) 
		{
		    printf("\n");
		}
	}    
}

int GetMaskValue(BYTE *MaskCenterPtr, int MaskLength, int MaskPtrShiftArr[], BYTE OutArr[])
{   //利用 MaskCenterPtr + MaskPtrShiftArr 取出 MaskParentData 中的值
    //[I/ ] MaskCenterPtr   : 設定 Mask 中心指標位置 
    //[I/ ] MaskLength      : 輸入 Mask 資料長度
    //[I/ ] MaskPtrShiftArr : 設定參考 MaskPtr 偏移表
    //[ /O] OutArr          : 輸出 MaskParentData 值
    //[ /O] ReturnValue     : 取值結果. [1]: 取值成功
    for(int i=0;i<MaskLength;i++)
        OutArr[i]=MaskCenterPtr[MaskPtrShiftArr[i]];
    return 1;
}

int MyMakeMaskPtrShiftArr(int MaskPtrShiftArr[], int MaskWH, int ImageRowPitch)
{   //製作 MaskPtr 偏移表, 供 Mask 撈值用.
    //[I/O] MaskShiftArr  : 提供一陣列, 存放 MaskPtrShift 值
    //[I/ ] MaskWH        : MaskPtrShift Table 寬高
    //[I/ ] ImageRowPitch : Mask Parent Data ROW 寬度 
    //[ /O] ReturnValue   : MyMakeMaskPtrShiftArr 製作結果
    //                      [1]: 製作成功
    //                      [2]: Error-Mask寬度不為奇數
    //                      [3]: Error-影像寬度Pitch小於Mask寬度
    
    //資料防呆
	if(MaskWH%2==0)
	    return 2; // ErrorCode[2]: Mask寬度不為奇數  
    if(ImageRowPitch<MaskWH)
	    return 3; // ErrorCode[3]: 影像寬度Pitch小於Mask寬度
		
	int HalfMaskWH = (MaskWH-1)/2; 	
	int Length     = MaskWH*MaskWH;
    int OrgPosPtr  = 0-HalfMaskWH*ImageRowPitch-HalfMaskWH;
	
	for(int i=0;i<Length;i++)
	{
		MaskPtrShiftArr[i]=OrgPosPtr+(i/MaskWH)*ImageRowPitch+(i%MaskWH);
	}
		
 	return 1; //製作成功
}

int main()
{
    printf("\n\n\n");
    int ImageWH=11;
    int MaskWH=3;
    BYTE ImageMem[ImageWH*ImageWH];
    int MaskPosShiftArr[MaskWH*MaskWH]; //指標shit寬度會>byte, 故用int
    BYTE MaskArr[MaskWH*MaskWH];

	//Step1. 初始化ImageMem Data
    int Temp=0;   
    for(int i=0;i<ImageWH*ImageWH;i++)
    {
        //Temp=i%ImageWH; ImageMem[i] = Temp;  //順序填值
        ImageMem[i] = rand()%256;            //亂數填值
    }
	printf("===測試影像資料(random)===\n");
	PrintMaskValue(ImageMem,ImageWH);
	printf("\n\n\n");
	
	//Step2. 產生 MaskPtr Shift Table
	MyMakeMaskPtrShiftArr(MaskPosShiftArr,MaskWH,ImageWH);
	printf("===MaskPtr Shift Table (size=%d*%d)===\n",MaskWH,MaskWH);
    PrintMaskValue(MaskPosShiftArr,MaskWH);
    printf("\n\n\n");
    
    //Step3. 設定Mask中心座標, 並取出ImageMem的值
    int MaskCenterPosX=4;
    int MaskCenterPosY=4;
    BYTE *MaskCenterPtrPos=ImageMem+(MaskCenterPosY*ImageWH)+MaskCenterPosX;
    GetMaskValue(MaskCenterPtrPos, MaskWH*MaskWH, MaskPosShiftArr, MaskArr);
    printf("===將Mask中心設在(%d,%d)並取出ImageMem的值===\n",MaskCenterPosX,MaskCenterPosY);
    PrintMaskValue(MaskArr,MaskWH);
    printf("\n\n\n");

    return 0;
}