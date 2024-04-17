# By工具箱

## 快捷键功能

### 自定义功能
可在程序中自定义功能类，继承ShortcutKeyItem即可。
```c++
void Core::loadShortcutKeyItems()
{
	ShortcutKeyMgr.add(std::make_shared<RunTQ>(L"运行天擎"));
	ShortcutKeyMgr.add(std::make_shared<CloseSP>(L"关闭自保"));
	ShortcutKeyMgr.add(std::make_shared<CloseTQ>(L"关闭天擎"));
	ShortcutKeyMgr.add(std::make_shared<CloseSPAndReplaceSafeBase>(L"替换SafeBase.dll"));
}
```

<img style="clear:both; display:block; margin:auto;" src="https://note.youdao.com/yws/api/personal/file/WEB20b0a968d258713a50836175f1a00990?method=download&shareKey=1ceab90219a4c93e41a15aa7544a871b" width="60%" />

## 便签功能

是一个极简的笔记本功能，5s自动保存内容。

<img style="clear:both; display:block; margin:auto;" src="https://note.youdao.com/yws/api/personal/file/WEB46f19fb505d4b3aaeff3d8edb38a2c39?method=download&shareKey=69927159fcbcf0bb1dc15e8cec5e65cf" width="60%" />