#include <Windows.h>
#include <io.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

void print(const char *error) {
	FILE *f; fopen_s(&f, "%APPDATA%/gvimbackground.log", "w"); fprintf(f, error); fclose(f);
}

int main(int argc, char *argv[]) { // argv[1] example: ab12ef; argv[2] example: 0.2 (should be in the range [0.0, 1.0]; argv[3] example: E:\1.png

	Sleep(1000); // Make sure when the program start, the gvim window has displayed.
	int scanfnum;

	if (argc < 4) { print("Too few arguments!\n"); return 1; }
	int colornum; scanfnum = sscanf_s(argv[1], "%x", &colornum); // Get the rgb number parameter color indicates
	if (scanfnum < 1) { print("Illegal color value!"); return 1; }
	int r = (colornum & 0xff0000) >> 16, g = (colornum & 0xff00) >> 8, b = colornum & 0xff;
	float alpha; scanfnum = sscanf_s(argv[2], "%f", &alpha);
	if (scanfnum < 1 || alpha < 0 || alpha > 1) { print("Illegal alpha value!"); return 1; }
	
	HWND vim = GetForegroundWindow(); // Make the background of vim transparent
	char name[100]; GetWindowText(vim, name, 100);
	if (! vim || ! strstr(name, "GVIM")) { print("The foreground window is not gvim!"); return 1; }
	if (_access(argv[3], 0) == -1) { print("The image does not exist!"); return 1; }

	RECT winrect; POINT p;
	GetWindowRect(vim, &winrect); // Get vim window's position and size
	p.x = (winrect.right - winrect.left) / 2; p.y = (winrect.bottom - winrect.top) / 2;
	HWND textwin = ChildWindowFromPoint(vim, p);
	if (!textwin) { print("Can not find the edit area!"); return 1; }
	SetWindowLongPtr(vim, GWL_EXSTYLE, GetWindowLong(vim, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(vim, RGB(r, g, b), NULL, LWA_COLORKEY);

	const char *windowname = "vimbackground"; // Prepare opencv window
	cv::namedWindow(windowname);
	HWND imgw = GetParent((HWND)cvGetWindowHandle(windowname));
	ShowWindow(imgw, SW_HIDE);
	SetWindowLongPtr(imgw, GWL_EXSTYLE, GetWindowLongPtr(imgw, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
	SetWindowLongPtr(imgw, GWL_STYLE, GetWindowLongPtr(imgw, GWL_STYLE) & ~WS_CAPTION);

	cv::Mat img = cv::imread(argv[3]);
	cv::Mat color(img.rows, img.cols, CV_8UC3, cv::Scalar(b, g, r));
	cv::addWeighted(img, alpha, color, 1 - alpha, 0.0, img);

	while (true) {

		cv::waitKey(200); // Make sure opencv window alive

		if (! IsWindow(vim)) break; // If the vim window has been closed
		if (IsIconic(vim)) ShowWindow(imgw, SW_HIDE); else {

			RECT textrect;
			GetWindowRect(textwin, &textrect); // Get text area's position and size
			int textwidth = textrect.right - textrect.left, textheight = textrect.bottom - textrect.top;

			cv::Mat resizeimg; // Change image's size
			cv::resize(img, resizeimg, cv::Size(textwidth, textheight));
			cv::imshow(windowname, resizeimg);

			bool f = GetForegroundWindow() == vim; // Transform opencv's window and make sure that vim window never gives the focus to opencv window
			SetWindowPos(imgw, vim, textrect.left, textrect.top, textwidth, textheight, 0);
			ShowWindow(imgw, SW_SHOW);
			if (f) SetForegroundWindow(vim);

		}
	}

	cv::destroyWindow(windowname);
	return 0;

}
