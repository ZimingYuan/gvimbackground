# GVim Background
[中文](https://github.com/ZimingYuan/gvimbackground/blob/master/README_zh.md)
Use image as gvim(Windows)'s background. Using WindowsAPI and OpenCV library.
### Usage(4 steps):
1. Put the 'gvimbackground.exe' and the 'opencv_world401.dll' into the same folder as 'gvim.exe'.
2. Put the 'gvimbackground.vim' into your 'plugin' folder.
3. Add four lines into your '_vimrc':
```
let Alpha = '0.2'
let Path = 'D:\1.jpg'
let MidX = '960'
let MidY = '540'
```
* '0.2' means the mixing ratio of your image and the original background color. If you set it to 0.0, the image will not display. If you set it to 1.0, you will see the original image. You can change it to any value in range [0.0, 1.0].
* 'D:\1.jpg' means the path of the image you want to use as your gvim's background.
* '960' and '540' means the middle point's position of scaling(shouldn't be larger than the image's width and height). You need to modify these two values according to your background image's resolution.
4. Enjoy coding on gvim with beautiful background!

![](https://github.com/ZimingYuan/gvimbackground/blob/master/example.png)
example(Smiling face background is from [learnopengl.com](https://learnopengl.com/img/textures/awesomeface.png))
### Notice:
1. This plugin can only run on 64-bit Windows and is only for gvim instead of terminal vim. If you want to use it on 32-bit windows, you can compile the 'gvimbackground.exe' youself(Using 32-bit OpenCV library).
2. When using this plugin, don't drag the gvim window too fast...
3. If the 'gvimbackground.exe' fail to execute, you can check its output in 'C:\gvimbackground.log' for reason.
### Update 2020.05.03:
Now this program will keep background image's resolution correct by scaling and croping.
