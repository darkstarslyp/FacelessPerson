#### 技术方案：
* Opencv图像处理
* 人脸关键点识别


#### 实现流程
> 这个方案主要采用提取静态场景以及后期根据人脸关键点识别合成图片，最终实现“无脸男”的效果。

![无脸男技术实现方案](http://upload-images.jianshu.io/upload_images/1592280-959486ace65a47e3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)



#### 核心代码
```
 if (landmarks.size() > 0&&!backgroundImg.empty()) {
                //one channel black image
                cv::Mat mask = cv::Mat::zeros(srcImg.size(), CV_8UC1);
               //置人脸区域的颜色为白色
                cv::fillConvexPoly(mask, landmarks.data(), landmarks.size(), cv::Scalar(255));
                 
               //通过findContours寻找人脸区域的轮廓，approxPolyDP对图像轮廓点进行多边形拟合。
                std::vector<cv::Point> contour;
                std::vector<std::vector<cv::Point>> contours;
                cv::findContours(mask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS);
                cv::approxPolyDP(contours[0], contour, 1.0, true);

                int width = srcImg.cols;
                int height = srcImg.rows;
               //将人脸区域内的像素点替换为背景图像的像素
                for(int i=1;i<=height;i++){
                    for(int j=1;j<=width;j++){
                        cv::Point point(j,i);
                        if(cv::pointPolygonTest(contour,point,false)>=0){
                            srcImg.at<cv::Vec4b>(i,j) =  backgroundImg.at<cv::Vec4b>(i,j);
                        }
                    }
                }
 }
```

#### 如何使用

```
git clone --recursive https://github.com/darkstarslyp/FacelessPerson.git
cd FacelessPerson
run.sh
```


#### 第三方依赖
1. [SDM](https://github.com/RoboPai/sdm)
2. Opencv
3. cmake 
