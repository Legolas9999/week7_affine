#include<iostream>
#include<opencv2/opencv.hpp>
using namespace cv;

int main() {
	//-------------------------------edge:Canny����-------------------------
	VideoCapture cap;
	cap.open(0);
	Mat frame;
	Mat frame_grey;
	Mat edge;
	while (1) {
		cap >> frame;
		cvtColor(frame,frame_grey,CV_BGR2GRAY); //��ͼ��ת���ɻҶ�ͼ
		Canny(frame_grey,edge,20,120,3,false);//����Canny���Ӻ���
		imshow("edge",edge);
		waitKey(1);
	}

	//----------------------------------------ͼ����ת--------------------------
	Mat hsv;
	Mat hsv_trans;
	hsv = imread("hsv.jpg");
	float angle = -10;		//������ת�Ƕ�
	float scale = 1;
	Point2f center(hsv.cols*0.5,hsv.rows*0.5);	//������ת����
	const Mat trans_matrix=getRotationMatrix2D(center,angle,scale);//���ú����õ��任����
	warpAffine(hsv,hsv_trans,trans_matrix,hsv.size());//����warpAffine����������ת
	imshow("hsv",hsv);
	imshow("hsv_trans",hsv_trans);
	

	//---------------------����任����Ҫ3����----------------------------
	Mat hsv_affine;
	Point2f hsv_pt[] = {					//ԭͼ���������
					Point2f(200,200),
					Point2f(250,200),
					Point2f(200,100),

	};

	Point2f affine_pt[] = {					//Ŀ��ͼ���������
					Point2f(300,100),
					Point2f(300,50),
					Point2f(200,100),
	};

	Mat affine_matrix = getAffineTransform(hsv_pt,affine_pt);  //�õ�����任�ı任����
	warpAffine(hsv,hsv_affine,affine_matrix,hsv.size());	//����warpAffine�������з���任
	imshow("affine_trans",hsv_affine);
	
	////------------------ͶӰ�任����Ҫ4����--------------------------------------------
	Mat project;
	Point2f pts1[] = {						//ԭͼ����ĸ���
					Point2f(150,150),
					Point2f(150,300),
					Point2f(350,300),
					Point2f(350,150),

	};
	Point2f pts2[] = {						//Ŀ��ͼ���4����
					Point2f(200,150),
					Point2f(200,300),
					Point2f(340,270),
					Point2f(340,180),

	};
	Mat perspective_matrix = getPerspectiveTransform(pts1,pts2);	//�õ�ͶӰ�任����
	warpPerspective(hsv,project,perspective_matrix,hsv.size());		//����warpPerspective��������ͶӰ�任
	imshow("project",project);


	//-----------------------��ϰ5����Ŀ��ͼ��lenת��----------------------------------
	Mat len;
	
	len = imread("len.jpg");			//����ԭͼ��
	int height = len.rows;				//�õ�ԭͼ����и�
	int length = len.cols;				//�õ�ԭͼ����п�
	Mat len_trans;						//Ŀ������ͼ��
	len_trans.create(height,length,len.type());
	//ÿ�����ص�����ͨ���ĻҶ�ֵ
	int pixel0;		//B
	int pixel1;		//G
	int pixel2;		//R

	Point2f pt1;		//��Ҫ���ĸ���
	Point2f pt2;
	Point2f pt3;
	Point2f pt4;
	
	for (int j=0; j < length; j++)				//Ѱ��ԭͼ�����Ͻǵĵ�
		{
					//��row ,col��
			pixel0 = len.at<Vec3b>(0, j)[0];	

			if (pixel0 <255)		//��ĳһͨ���Ҷ�ֵС��255���¼�õ�λ�õ�pt1.���˳�ѭ��
			{
				pt1.x = j;
				pt1.y = 0;
				break;
			}

		}
	
	for (int j=0; j < length; j++)				//Ѱ��ԭͼ�����½ǵĵ�
	{
		
		pixel0 = len.at<Vec3b>(height - 1, j)[0];	//����ͨ���ĻҶ�ֵ
		pixel1 = len.at<Vec3b>(height - 1, j)[1];
		pixel2 = len.at<Vec3b>(height - 1, j)[2];
		
		if (((pixel0 + pixel1 + pixel2) / 3) < 120)	//������ͨ���Ҷ�ֵ��ƽ��ֵС��120ʱ����¼�õ�λ�õ�pt2�����˳�ѭ��
		{
			pt2.x = j;
			pt2.y = height-1;
			break;
		}
	}
	
	for (int j=0; j < height; j++)			//Ѱ��ԭͼ�����½ǵĵ�
	{
		pixel0 = len.at<Vec3b>(j,0 )[0];//����ͨ���ĻҶ�ֵ
		pixel1 = len.at<Vec3b>(j, 0)[1];
		pixel2 = len.at<Vec3b>(j, 0)[2];
		if (((pixel0+pixel1+pixel2)/3)<150)	//������ͨ���Ҷ�ֵ��ƽ��ֵС��150�����¼�õ�����굽pt3�����˳�ѭ��
		{
			pt3.x = 0;
			pt3.y = j;
			break;
		}
	}
	for (int j = 0; j < height; j++)	//Ѱ��ԭͼ�������Ͻǵĵ�
	{
		
		pixel0 = len.at<Vec3b>(j, length - 1)[0];	//����ͨ���Ҷ�ֵ
		pixel1 = len.at<Vec3b>(j, length - 1)[1];
		pixel2 = len.at<Vec3b>(j, length - 1)[2];
		if (((pixel0 + pixel1 + pixel2) / 3) < 150)		//������ͨ���ĻҶ�ֵ��ƽ��ֵС��150�����¼�õ�λ�õ�pt4�����˳�ѭ��
		{
			pt4.x = length-1;
			pt4.y = j;
			break;
		}
	}
	

	Point2f pt_len1[] = {						//��¼����4��ԭͼ���еĶ���
					Point2f(pt1.x,pt1.y),
					Point2f(pt2.x,pt2.y),
					Point2f(pt3.x,pt3.y),
					Point2f(pt4.x,pt4.y),
					

	};
	Point2f pt_len2[] = {
		Point2f(0,0),							//4������Ŀ��ͼ��������Ӧ�ĵ�
		Point2f(length - 1,height - 1),
		Point2f(0,height - 1),
		Point2f(length-1,0),
					
					
					
					
	};
	
	Mat len_perspective_matrix = getPerspectiveTransform(pt_len1, pt_len2);		//�õ�Ͷ��任����
	warpPerspective(len,len_trans,len_perspective_matrix,len.size());			//����warpPerspective��������͸�ӱ任
	imshow("len_trans",len_trans);			//���Ŀ��ͼ��

	
	waitKey(0);

}