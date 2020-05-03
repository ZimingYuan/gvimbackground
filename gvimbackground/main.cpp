#include <io.h>
#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

void print(const char *error) {
	FILE *f; fopen_s(&f, "C:/gvimbackground.log", "w"); fprintf(f, error); fclose(f);
}

bool Rectcmp(const RECT &a, const RECT &b) {
	return a.left == b.left && a.right == b.right && a.top == b.top && a.bottom == b.bottom;
}

int main(int argc, char *argv[]) {
	// argv[1] is the color to replace in hex like ab12ef;
	// argv[2] is the alpha value like 0.2 (should be in the range [0.0, 1.0];
	// argv[3] is the image's path like E:\1.png;
	// argv[4] and argv[5] is the image's middle point of cropping like 1000 500;

	Sleep(1000); // Make sure when the program start, the gvim window has displayed.
	// Read the args.
	int scanfnum;
	if (argc < 6) { print("Too few arguments!\n"); return 1; }
	int colornum; scanfnum = sscanf_s(argv[1], "%x", &colornum); // Get the rgb number parameter color indicates
	if (scanfnum < 1) { print("Illegal color value!"); return 1; }
	int r = (colornum & 0xff0000) >> 16, g = (colornum & 0xff00) >> 8, b = colornum & 0xff;
	float alpha; scanfnum = sscanf_s(argv[2], "%f", &alpha);
	if (scanfnum < 1 || alpha < 0 || alpha > 1) { print("Illegal alpha value!"); return 1; }
	int midx; scanfnum = sscanf_s(argv[4], "%d", &midx);
	int midy; scanfnum = sscanf_s(argv[5], "%d", &midy);
	
	// Make the background of vim transparent and get windows' handles.
	HWND vim = GetForegroundWindow();
	char name[100]; GetWindowText(vim, name, 100);
	if (! vim || ! strstr(name, "GVIM")) { print("The foreground window is not gvim!"); return 1; }
	cv::Mat img = cv::imread(argv[3]), resizeimg;
	if (! img.data) { print("The image does not exist!"); return 1; }
	if (midx < 0 || midy < 0 || midx > img.cols || midy > img.cols) {
		print("The middle point is out of the image!"); return 1;
	}
	RECT winrect; POINT p;
	GetWindowRect(vim, &winrect); // Get vim window's position and size
	p.x = (winrect.right - winrect.left) / 2; p.y = (winrect.bottom - winrect.top) / 2;
	HWND textwin = ChildWindowFromPoint(vim, p);
	if (!textwin) { print("Can not find the edit area!"); return 1; }
	SetWindowLongPtr(vim, GWL_EXSTYLE, GetWindowLong(vim, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(vim, RGB(r, g, b), NULL, LWA_COLORKEY);

	// Build OpenCV window.
	const char *windowname = "gvimbackground"; // Prepare opencv window
	cv::namedWindow(windowname);
	HWND imgw = GetParent((HWND)cvGetWindowHandle(windowname));
	ShowWindow(imgw, SW_HIDE);
	SetWindowLongPtr(imgw, GWL_EXSTYLE, GetWindowLongPtr(imgw, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
	SetWindowLongPtr(imgw, GWL_STYLE, GetWindowLongPtr(imgw, GWL_STYLE) & ~WS_CAPTION);

	// Alpha blending.
	cv::Mat color(img.rows, img.cols, CV_8UC3, cv::Scalar(b, g, r));
	cv::addWeighted(img, alpha, color, 1 - alpha, 0.0, img);
	int minx = std::min(midx, img.cols - midx) * 2, miny = std::min(midy, img.rows - midy) * 2;
	bool visible = false;
	bool topisvim = false;

	while (true) {

		cv::waitKey(100); // Make sure opencv window alive
		if (! IsWindow(vim)) break; // If the vim window has been closed

		if (IsIconic(vim)) { // If the vim window is minimized
			if (visible) {
				ShowWindow(imgw, SW_HIDE);
				visible = false;
			}
		} else {

			RECT textrect;
			GetWindowRect(textwin, &textrect); // Get text area's position and size
			bool f = GetForegroundWindow() == vim; // Transform opencv's window and make sure that vim window never gives the focus to opencv window
			if (! visible || ! Rectcmp(winrect, textrect) || (!topisvim && f)) {
				if (!visible) {
					ShowWindow(imgw, SW_SHOW);
					visible = true;
				}
				winrect = textrect;
				int winwidth = winrect.right - winrect.left, winheight = winrect.bottom - winrect.top;
				cv::Rect crop;
				if (minx >= (double)miny * winwidth / winheight) {
					int w = (int)((double)miny * winwidth / winheight);
					crop = cv::Rect(midx - w / 2, midy - miny / 2, w, miny);
				} else {
					int h = (int)((double)minx * winheight / winwidth);
					crop = cv::Rect(midx - minx / 2, midy - h / 2, minx, h);
				}
				cv::resize(cv::Mat(img, crop), resizeimg, cv::Size(winwidth, winheight));
				cv::imshow(windowname, resizeimg);
				SetWindowPos(imgw, vim, winrect.left, winrect.top, winwidth, winheight, 0);
				SetForegroundWindow(vim);
			}
			topisvim = f;
		}
	}

	cv::destroyWindow(windowname);
	return 0;

}
