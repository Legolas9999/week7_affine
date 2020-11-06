#include<iostream>
#include<opencv2/opencv.hpp>
using namespace cv;

int main() {
	//-------------------------------edge:Canny算子-------------------------
	VideoCapture cap;
	cap.open(0);
	Mat frame;
	Mat frame_grey;
	Mat edge;
	while (1) {
		cap >> frame;
		cvtColor(frame,frame_grey,CV_BGR2GRAY); //把图像转换成灰度图
		Canny(frame_grey,edge,20,120,3,false);//调用Canny算子函数
		imshow("edge",edge);
		waitKey(1);
	}

	//----------------------------------------图像旋转--------------------------
	Mat hsv;
	Mat hsv_trans;
	hsv = imread("hsv.jpg");
	float angle = -10;		//设置旋转角度
	float scale = 1;
	Point2f center(hsv.cols*0.5,hsv.rows*0.5);	//设置旋转中心
	const Mat trans_matrix=getRotationMatrix2D(center,angle,scale);//调用函数得到变换矩阵
	warpAffine(hsv,hsv_trans,trans_matrix,hsv.size());//调用warpAffine函数进行旋转
	imshow("hsv",hsv);
	imshow("hsv_trans",hsv_trans);
	

	//---------------------仿射变换：需要3个点----------------------------
	Mat hsv_affine;
	Point2f hsv_pt[] = {					//原图像的三个点
					Point2f(200,200),
					Point2f(250,200),
					Point2f(200,100),

	};

	Point2f affine_pt[] = {					//目标图像的三个点
					Point2f(300,100),
					Point2f(300,50),
					Point2f(200,100),
	};

	Mat affine_matrix = getAffineTransform(hsv_pt,affine_pt);  //得到仿射变换的变换矩阵
	warpAffine(hsv,hsv_affine,affine_matrix,hsv.size());	//调用warpAffine函数进行仿射变换
	imshow("affine_trans",hsv_affine);
	
	////------------------投影变换：需要4个点--------------------------------------------
	Mat project;
	Point2f pts1[] = {						//原图像的四个点
					Point2f(150,150),
					Point2f(150,300),
					Point2f(350,300),
					Point2f(350,150),

	};
	Point2f pts2[] = {						//目标图像的4个点
					Point2f(200,150),
					Point2f(200,300),
					Point2f(340,270),
					Point2f(340,180),

	};
	Mat perspective_matrix = getPerspectiveTransform(pts1,pts2);	//得到投影变换矩阵
	warpPerspective(hsv,project,perspective_matrix,hsv.size());		//调用warpPerspective函数进行投影变换
	imshow("project",project);


	//-----------------------练习5：把目标图像len转正----------------------------------
	Mat len;
	
	len = imread("len.jpg");			//读入原图像
	int height = len.rows;				//得到原图像的行高
	int length = len.cols;				//得到原图像的列宽
	Mat len_trans;						//目标生成图像
	len_trans.create(height,length,len.type());
	//每个像素的三个通道的灰度值
	int pixel0;		//B
	int pixel1;		//G
	int pixel2;		//R

	Point2f pt1;		//需要的四个点
	Point2f pt2;
	Point2f pt3;
	Point2f pt4;
	
	for (int j=0; j < length; j++)				//寻找原图像左上角的点
		{
					//（row ,col）
			pixel0 = len.at<Vec3b>(0, j)[0];	

			if (pixel0 <255)		//若某一通道灰度值小于255则记录该点位置到pt1.并退出循环
			{
				pt1.x = j;
				pt1.y = 0;
				break;
			}

		}
	
	for (int j=0; j < length; j++)				//寻找原图像右下角的点
	{
		
		pixel0 = len.at<Vec3b>(height - 1, j)[0];	//三个通道的灰度值
		pixel1 = len.at<Vec3b>(height - 1, j)[1];
		pixel2 = len.at<Vec3b>(height - 1, j)[2];
		
		if (((pixel0 + pixel1 + pixel2) / 3) < 120)	//若三个通道灰度值的平均值小于120时，记录该点位置到pt2，并退出循环
		{
			pt2.x = j;
			pt2.y = height-1;
			break;
		}
	}
	
	for (int j=0; j < height; j++)			//寻找原图像左下角的点
	{
		pixel0 = len.at<Vec3b>(j,0 )[0];//三个通道的灰度值
		pixel1 = len.at<Vec3b>(j, 0)[1];
		pixel2 = len.at<Vec3b>(j, 0)[2];
		if (((pixel0+pixel1+pixel2)/3)<150)	//若三个通道灰度值的平均值小于150，则记录该点的坐标到pt3，并退出循环
		{
			pt3.x = 0;
			pt3.y = j;
			break;
		}
	}
	for (int j = 0; j < height; j++)	//寻找原图像中右上角的点
	{
		
		pixel0 = len.at<Vec3b>(j, length - 1)[0];	//三个通道灰度值
		pixel1 = len.at<Vec3b>(j, length - 1)[1];
		pixel2 = len.at<Vec3b>(j, length - 1)[2];
		if (((pixel0 + pixel1 + pixel2) / 3) < 150)		//若三个通道的灰度值的平均值小于150，则记录该点位置到pt4，并退出循环
		{
			pt4.x = length-1;
			pt4.y = j;
			break;
		}
	}
	

	Point2f pt_len1[] = {						//记录到的4个原图像中的顶点
					Point2f(pt1.x,pt1.y),
					Point2f(pt2.x,pt2.y),
					Point2f(pt3.x,pt3.y),
					Point2f(pt4.x,pt4.y),
					

	};
	Point2f pt_len2[] = {
		Point2f(0,0),							//4个点在目标图像中所对应的点
		Point2f(length - 1,height - 1),
		Point2f(0,height - 1),
		Point2f(length-1,0),
					
					
					
					
	};
	
	Mat len_perspective_matrix = getPerspectiveTransform(pt_len1, pt_len2);		//得到投射变换矩阵
	warpPerspective(len,len_trans,len_perspective_matrix,len.size());			//调用warpPerspective函数进行透视变换
	imshow("len_trans",len_trans);			//输出目标图像

	
	waitKey(0);

}