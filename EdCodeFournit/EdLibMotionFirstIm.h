#pragma once
int MotionDetectionRefFirstIm(EdIMAGE *image, EdIMAGE *imres, EdIMAGE *imref, unsigned char seuil);
int MotionDetectionRefresh(EdIMAGE *image, EdIMAGE *imres, EdIMAGE *imref, unsigned char Seuil , float Oublie);