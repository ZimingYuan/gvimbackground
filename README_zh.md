# GVim Background
用图片作为gvim（Windows）的背景。使用了WindowsAPI和OpenCV库.
### 用法（3步）:
1. 把gvimbackground.exe和opencv_world401.dll放到gvim.exe所在的目录下。
2. 把gvimbackground.vim放到plugin文件夹。
3. 把下列两行加到_vimrc里：
```
let Alpha = '0.2'
let Path = 'D:\1.jpg'
call foreground()
```
* '0.2'表示图片和原来你用的主题的背景色的混合比例。如果你把这个值设成0.0图片就不会显示，如果你把这个值设成1.0，则直接展示原图（不混合）。你可以把这个值设为[0.0, 1.0]中的任意数。
* 'D:\1.jpg' 表示你想用作背景的图片的路径。

![](https://github.com/ZimingYuan/gvimbackground/blob/master/example.png)
例子（这幅图我用的背景来自[learnopengl.com](https://learnopengl.com/img/textures/awesomeface.png)）
### 注意:
1. 这个插件只能用在64位的Windows下面，且只针对于gvim，对命令行的vim不起效果。如果你想把这个插件用在32位的Windows下面，请自行编译gvimbackground.exe（需要用到32位的OpenCV库）。
2. 使用这个插件时拖动gvim窗口别太快，不然会有很奇怪的效果。
3. 如果gvimbackground.exe启动失败了，你可以去%APPDATA%\gvimbackground.log这个文件中看程序输出找原因。

