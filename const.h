#ifndef CONST_H
#define CONST_H

enum TreeItemType
{
    TreeItemProject=1, //表示项目类型
    TreeItemDirectory=2, //表示子文件夹类型
    TreeItemPicture=3 //表示图片
};

enum PictureButtonState
{
    //播放状态
    NormalPlayState=1,
    HoverPlayState=2,
    PressedPlayState=3,

    //暂停状态
    NormalPauseState=4,
    HoverPauseState=5,
    PressedPaudeState=6
};

const int PROGRESS_LENGTH=300;
const int PROGRESS_MAX=300;

const int PREVIEW_SIZE=100; //previewlistWidget项目大小
const int PREVIEWICON_SIZE=90;
#endif // CONST_H
