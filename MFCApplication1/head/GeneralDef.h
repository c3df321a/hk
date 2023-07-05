
#ifndef GENERALDEF_H
#define GENERALDEF_H

#include "HCNetSDK.h"
#include "plaympeg4.h"

#define MAX_CHANS      24              //�豸֧�ֵ����ͨ����
#define MAX_DEVICE     10             //����豸��
#define MAX_OUTPUTS	   16	//16���Ĵ�����
#define MAX_PREVIEWS		512	//��Ԥ���������

#define MAX_CRUISE_SEQ  32
#define MAX_CRUISE_POINT 32
#define MAX_CRUISE_PRESET 128
#define MAX_CRUISE_TIME 255
#define MAX_CRUISE_SPEED 15

//�豸��������
#define TREE_ALL_T	 0        //������ڵ�
#define DEVICETYPE 	 1		  //�����豸
#define CHANNELTYPE	 2	      //����ͨ��


#define DEMO_FULL_WIDTH 1024	//demoĬ�ϵĿ��
#define DEMO_FULL_HEIGHT 768	//demoĬ�ϵĸ߶�
#define OUTPUT_INTERVAL 4		//ÿ������ļ����С


//��ʱ��ʱ��
#define CYCLE_PREVIEW_TIMER 2    //ѭ��Ԥ����ʱ��

typedef struct STRU_CHANNEL_INFO
{
	char    chChanName[40];     //ͨ������
	int		iChanIndex;			//���ͨ���� = ��������+startchan
	int		iPicResolution;				//ͼƬ�ֱ���
	int		iPicQuality;				//ͼƬ����
	char	chAccessChanIP[16];     //ip����ͨ����ip��ַ
	BOOL    bEnable;              //�Ƿ���Ч
    NET_DVR_DECODERCFG_V30  struDecodercfg;   //ͨ���Ľ�������Ϣ
	STRU_CHANNEL_INFO()
	{
		chChanName[0]       ='\0';
		iChanIndex          = -1;
		iPicResolution		= 0;
		iPicQuality			= 2;
		chAccessChanIP[0]	= '\0';	
		bEnable             = FALSE;
	}
}CHANNEL_INFO,*pCHANNEL_INFO;

typedef struct STRU_DEVICE_INFO
{

	LONG    lLoginID;
	int		iDeviceChanNum;		    //�豸��ͨ����
	int		iStartChan;				//�豸��ʼͨ����
	int 	iIPChanNum;				//�������ͨ������
    int 	iIPStartChan;			//����ͨ����ʼͨ����
	int		iEnableChanNum;			//��Чͨ����
	BOOL    bIPRet;                 //�Ƿ�֧��ip����
	
	CHANNEL_INFO struChanInfo[MAX_CHANNUM_V30];


	STRU_DEVICE_INFO()
	{
		 lLoginID = -1;
		 iDeviceChanNum      =  -1;
		 iStartChan          = 0;
		 iIPChanNum          = 0;
		 iEnableChanNum      = -1;
		 bIPRet              = FALSE;
	}	 
}LOCAL_DEVICE_INFO,*pLOCAL_DEVICE_INFO;




#endif