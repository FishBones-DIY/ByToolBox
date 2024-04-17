#include "sticky_note_tab_widget.h"

#include <set>
#include <algorithm>

#include <QDir>
#include <QFile>
#include <QVariant>
#include <QFileInfo>
#include <QTextCodec>
#include <QScrollBar>
#include <QPushButton>
#include <QScrollArea>
#include <QTextLayout>

#include "common/public/log.h"
#include "common/uitils/path_utils.h"
#include "common/uitils/string_utils.h"
#include "ui/message_box/by_messagebox.h"
#include "base_window/include/base_window.h"

StickyNoteTabWidget::StickyNoteTabWidget(QWidget* parent)
	:QWidget(parent)
{
	parent_widget_ = parent;
	note_file_save_dir_ = QDir::currentPath() + "\\note";
	confirmFolder(note_file_save_dir_);

	bar_group_ = new QButtonGroup();

	QHBoxLayout* note_layout = new QHBoxLayout();
	note_layout->setContentsMargins(0, 0, 0, 0);
	note_layout->setSpacing(0);

	QWidget* note_bar_widget = createNoteBarWidget();
	note_content_widget_ = createNoteContentWidger();

	note_layout->addWidget(note_bar_widget);
	note_layout->addWidget(note_content_widget_);

	{
		//设置等比伸缩
		QSizePolicy note_bar_widget_size_policy = note_bar_widget->sizePolicy();
		QSizePolicy note_content_widget_size_policy = note_content_widget_->sizePolicy();

		note_bar_widget_size_policy.setHorizontalStretch(1);
		note_content_widget_size_policy.setHorizontalStretch(3);

		note_bar_widget->setSizePolicy(note_bar_widget_size_policy);
		note_content_widget_->setSizePolicy(note_content_widget_size_policy);
	}

	this->setLayout(note_layout);

	loadLocalNotes();
	connect(title_name_edit_, &QLineEdit::textChanged, this, &StickyNoteTabWidget::onNoteTitleChanged);
	connect(content_edit_, &QTextEdit::textChanged, this, &StickyNoteTabWidget::onNoteTextChanged);

	// 创建右键菜单上移、下移、删除
	createNoteBarRightPressedMenu();
};

QWidget* StickyNoteTabWidget::createNoteBarWidget()
{
	QWidget* bar_widget = new QWidget();
	bar_widget->setWhatsThis("note_bar_widget");
	bar_widget_layout_ = new QVBoxLayout();
	bar_widget_layout_->setContentsMargins(0, 8, 0, 0);
	bar_widget_layout_->setSpacing(0);

	QPushButton* add_note = new QPushButton();
	add_note->setWhatsThis("note_add_button");

	QHBoxLayout* add_note_layout = new QHBoxLayout();
	add_note_layout->setContentsMargins(0, 0, 0, 0);
	add_note_layout->setSpacing(0);

	QLabel* add_note_image_label = new QLabel();
	add_note_image_label->setWhatsThis("note_add_image_label");
	add_note_image_label->setFixedWidth(20);
	add_note_image_label->setFixedHeight(20);
	QPixmap image_saved_image(":/public/add_note");
	int width = add_note_image_label->width();
	int height = add_note_image_label->height();
	image_saved_image = image_saved_image.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	add_note_image_label->setPixmap(image_saved_image);

	QLabel* add_note_text_label = new QLabel();
	add_note_text_label->setWhatsThis("note_add_text_label");
	add_note_text_label->setText(u8"新建");

	add_note_layout->addStretch();
	add_note_layout->addWidget(add_note_image_label);
	add_note_layout->addSpacing(10);
	add_note_layout->addWidget(add_note_text_label);
	add_note_layout->addStretch();

	add_note->setLayout(add_note_layout);

	connect(add_note, &QPushButton::clicked, this, &StickyNoteTabWidget::onNoteAddButtonClicked);

	// 加个壳可以使按钮对齐
	QVBoxLayout* bar_widget_layout_shell = new QVBoxLayout();
	bar_widget_layout_shell->setContentsMargins(0, 6, 0, 0);
	bar_widget_layout_shell->setSpacing(0);
	bar_widget_layout_shell->addWidget(add_note);
	bar_widget_layout_shell->addLayout(bar_widget_layout_);
	bar_widget_layout_shell->addStretch();


	bar_widget->setLayout(bar_widget_layout_shell);

	return BaseWindow<QWidget>::addScrollArea(bar_widget, "sticky_note_scroll_area");
}

void StickyNoteTabWidget::createNoteBarRightPressedMenu()
{
	note_bar_right_pressed_menu_ = new NoteTitleRightPressMenu(this);

	connect(note_bar_right_pressed_menu_, &NoteTitleRightPressMenu::moveUpPressed, this, &StickyNoteTabWidget::onNoteBarMoveUpClicked);
	connect(note_bar_right_pressed_menu_, &NoteTitleRightPressMenu::moveDownPressed, this, &StickyNoteTabWidget::onNoteBarMoveDownClicked);
	connect(note_bar_right_pressed_menu_, &NoteTitleRightPressMenu::moveDeletePressed, this, &StickyNoteTabWidget::onNoteBarMoveDeleteClicked);
}

void StickyNoteTabWidget::onNoteAddButtonClicked()
{
	std::string new_file_path = getNotePathFullPath(STRING_UNTITLED_NOTES + ".note").toLocal8Bit().data();
	QString file_name = QString::fromLocal8Bit(path_utils::getUnduplicatedFolderName(new_file_path).c_str());
	writeFile(file_name, "");
	int index = addNote(file_name);
	checkedNote(index);
	saveNoteFileList();
	content_edit_->setFocus();
}

void StickyNoteTabWidget::onNoteTitleEditFocusIn()
{
	// 编辑文件名的时候先不自动保存，不然搞得有点复杂了
	auto_asve_timer_pausing = true;

	if (path_utils::isUnduplicatedFolderName(STRING_UNTITLED_NOTES.toLocal8Bit().data()
		, title_name_edit_->text().toLocal8Bit().data()))
	{
		title_name_edit_->setText("");
	}
}

void StickyNoteTabWidget::onNoteTitleEditFocusOut()
{
	auto_asve_timer_pausing = false;

	QString title_text = title_name_edit_->text();
	if (title_text.isEmpty())
	{
		int checked_id = bar_group_->checkedId();
		title_name_edit_->setText(note_list_[checked_id].file_name);
	}
	else
	{
		static std::set<QChar> special_char_set{ '\\','/',':','*','?' ,'"' ,'<' ,'>','|' };
		QString new_text;
		for (const auto& c : title_text)
		{
			if (special_char_set.find(c) == special_char_set.end())
				new_text += c;
		}

		if (new_text != title_text)
		{
			title_name_edit_->setText(new_text);

			QString tips_content = u8"名称中不能含有特殊符号";
			for (const auto& special_char : special_char_set)
				tips_content += (' ' + special_char);
			MessageBox_By(parent_widget_, true, u8"重命名?", tips_content, u8"知道了", u8"");
		}
	}

	// 防止重置后为空
	if (title_name_edit_->text().isEmpty())
	{
		int checked_id = bar_group_->checkedId();
		title_name_edit_->setText(note_list_[checked_id].file_name);
	}
}

QWidget* StickyNoteTabWidget::createNoteContentWidger()
{
	QWidget* content_widget = new QWidget();
	content_widget->setWhatsThis("note_content_widget");
	QVBoxLayout* content_widget_layout = new QVBoxLayout();
	content_widget_layout->setContentsMargins(0, 0, 0, 0);
	content_widget_layout->setSpacing(0);

	QWidget* content_title_widget = new QWidget();
	content_title_widget->setFixedHeight(50);
	content_title_widget->setWhatsThis("note_title_widger");
	QHBoxLayout* content_title_layout = new QHBoxLayout();
	content_title_layout->setContentsMargins(0, 0, 0, 0);
	content_title_layout->setSpacing(0);

	// todo 添加标题输入和编辑保存按钮
	title_name_edit_ = new ByLineEdit();
	title_name_edit_->setWhatsThis("note_title_edit");
	// 感觉是qt的bug，不提前设置一下样式表不会触发获取真正的样式，在这里设置一下触发真正的样式来计算字体宽度。
	title_name_edit_->setStyleSheet("QLineEdit{font - family: \"宋体\";}");

	connect(title_name_edit_, &ByLineEdit::focusIn, this, &StickyNoteTabWidget::onNoteTitleEditFocusIn);
	connect(title_name_edit_, &ByLineEdit::focusOut, this, &StickyNoteTabWidget::onNoteTitleEditFocusOut);

	note_save_tips_button_ = createNoteSaveTipsButton();

	note_save_button_ = new QPushButton();
	note_save_button_->setText(u8"保存");
	note_save_button_->setWhatsThis("note_save_button");

	connect(note_save_button_, &QPushButton::pressed, this, &StickyNoteTabWidget::onNoteSaveButtonClicked);

	content_title_layout->addWidget(title_name_edit_);
	content_title_layout->addWidget(note_save_tips_button_);
	content_title_layout->addStretch();
	content_title_layout->addWidget(note_save_button_);

	content_title_widget->setLayout(content_title_layout);

	content_edit_ = new ByTextEdit();
	content_edit_->setWhatsThis("note_text_edit");
	content_edit_->setPlaceholderText(u8"输入");

	content_edit_->setFont(QFont("Microsoft JhengHei", 12));

	content_widget_layout->addWidget(content_title_widget);
	content_widget_layout->addWidget(content_edit_);

	content_widget->setLayout(content_widget_layout);

	return content_widget;
}

void StickyNoteTabWidget::onNoteSaveButtonClicked()
{
	int checked_id = bar_group_->checkedId();
	saveNote(checked_id);
}

void StickyNoteTabWidget::saveNote(int note_index)
{
	int checked_id = note_index;

	if (note_list_[checked_id].file_content == note_list_[checked_id].file_content_temp &&
		note_list_[checked_id].file_name == note_list_[checked_id].file_name_temp)
		return;

	note_list_[checked_id].file_content = note_list_[checked_id].file_content_temp;

	writeNote(checked_id);

	changeNoteFileName(note_list_[checked_id].file_name, note_list_[checked_id].file_name_temp);

	note_list_[checked_id].note_button->setText(QString::fromLocal8Bit(string_utils::trim(note_list_[checked_id].file_name_temp.toLocal8Bit().data()).c_str()));

	changeSaveStatues(checked_id);

	saveNoteFileList();
}

void StickyNoteTabWidget::saveAllNote()
{
	// 先用这种方法保存，有没有更合理的方法？
	for (int i = 0; i < note_list_.size(); ++i)
	{
		saveNote(i);
	}
}

void StickyNoteTabWidget::onNoteTitleChanged()
{
	int checked_id = bar_group_->checkedId();
	note_list_[checked_id].file_name_temp = title_name_edit_->text();

	if (isTitleChanged(checked_id))
		changeSaveImage(false);
	else
		changeSaveImage(true);

	adjustSizetTitleNameEdit();
}

void StickyNoteTabWidget::onNoteTextChanged()
{
	int checked_id = bar_group_->checkedId();
	note_list_[checked_id].file_content_temp = content_edit_->toPlainText();

	if (isContentChanged(checked_id))
		changeSaveImage(false);
	else
		changeSaveImage(true);
}

void StickyNoteTabWidget::changeSaveStatues(int note_index)
{
	if (isContentChanged(note_index) || isTitleChanged(note_index))
		changeSaveImage(false);
	else
		changeSaveImage(true);
}

bool StickyNoteTabWidget::isContentChanged(int note_index)
{
	return note_list_[note_index].file_content != note_list_[note_index].file_content_temp;
}

bool StickyNoteTabWidget::isTitleChanged(int note_index)
{
	return note_list_[note_index].file_name != note_list_[note_index].file_name_temp;
}

void StickyNoteTabWidget::changeSaveImage(bool saved)
{
	if (saved)
	{
		// 未发生变更
		note_save_tips_image_saved_->setVisible(true);
		note_save_tips_image_doing_save_->setVisible(false);
		note_save_tips_image_doing_save_->Stop();
		note_save_tips_content_->setText(u8"已保存");
	}
	else
	{
		// 发生变更
		note_save_tips_image_saved_->setVisible(false);
		note_save_tips_image_doing_save_->setVisible(true);
		note_save_tips_image_doing_save_->Play();
		note_save_tips_content_->setText(u8"编辑中");
	}
}

int StickyNoteTabWidget::getTitleNameEditTextWidth()
{
	return title_name_edit_->fontMetrics().width(title_name_edit_->text()) + 30;
}

void StickyNoteTabWidget::adjustSizetTitleNameEdit()
{
	int text_width = getTitleNameEditTextWidth();
	if (text_width < note_title_name_edit_max_width_)
		title_name_edit_->setFixedWidth(text_width);
	else
		title_name_edit_->setFixedWidth(note_title_name_edit_max_width_);
}

bool StickyNoteTabWidget::writeNote(int note_index)
{
	return writeFile(getNoteFileFullPath(note_list_[note_index].file_name)
		, note_list_[note_index].file_content);
}

bool StickyNoteTabWidget::writeFile(QString file_full_path, QString file_content)
{
	QFile file(file_full_path);
	bool res = file.open(QIODevice::WriteOnly | QIODevice::Text);

	QTextStream out(&file);
	out << file_content;
	out.flush();

	file.close();

	return res;
}

bool StickyNoteTabWidget::changeNoteFileName(QString& old_name, const QString& new_name)
{
	QString old_full_name = getNoteFileFullPath(old_name);
	QString new_full_name = getNoteFileFullPath(new_name);
	QFile file(old_full_name);
	bool res = file.rename(new_full_name);
	if (res)
		old_name = new_name;
	return res;
}

QString StickyNoteTabWidget::getNoteFileFullPath(const QString& file_name)
{
	return getNotePathFullPath(file_name) + ".note";
}

QString StickyNoteTabWidget::getNotePathFullPath(const QString& file_name)
{
	return note_file_save_dir_ + "\\" + file_name;
}

void StickyNoteTabWidget::loadLocalNotes()
{
	std::vector<QString> file_list = getNoteFileList();

	for (const auto& file : file_list)
	{
		addNote(getNoteFileFullPath(file));
	}

	if (note_list_.size() > 0)
		checkedNote(0);

	saveNoteFileList();
	startAutoSaveTimer();
}

std::vector<QString> StickyNoteTabWidget::getNoteFileList()
{
	std::vector<QString> file_list;
	QString note_list_name = "note.list";
	QFile note_list(getNotePathFullPath(note_list_name));

	if (note_list.exists() && note_list.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		while (!note_list.atEnd())
		{
			QTextCodec* codec = QTextCodec::codecForName("GBK");
			QString file_name = codec->toUnicode(note_list.readLine());

			if (file_name.endsWith("\n"))
				file_name.chop(1);

			if (QFile::exists(getNoteFileFullPath(file_name)))
			{
				// 内部去重
				auto ite = std::find(file_list.begin(), file_list.end(), file_name);
				if (ite == file_list.end())
					file_list.emplace_back(file_name);
			}
		}
		note_list.close();
	}

	QString note_dir = note_file_save_dir_;
	QDir dir(note_dir);
	QStringList name_filters;//设置文件过滤器
	name_filters << "*.note";//设置文件过滤格式
	QStringList note_files = dir.entryList(name_filters, QDir::Files | QDir::Readable, QDir::Name);
	for (const auto& note_file : note_files)
	{
		int extension_index = note_file.lastIndexOf(".");
		QString file_name = note_file.mid(0, extension_index);

		auto ite = std::find(file_list.begin(), file_list.end(), file_name);
		if (ite == file_list.end())
			file_list.emplace_back(file_name);
	}

	return file_list;
}

bool StickyNoteTabWidget::saveNoteFileList()
{
	QString file_list_string;
	for (const auto& note : note_list_)
	{
		file_list_string += note.file_name + "\n";
	}
	file_list_string.chop(1);

	return writeFile(getNotePathFullPath("note.list"), file_list_string);
}

int StickyNoteTabWidget::addNote(const QString& file_full_path)
{
	QString file_name;

	if (file_full_path.isEmpty())
	{
		file_name = STRING_UNTITLED_NOTES;
	}
	else
	{
		QFileInfo file_info(file_full_path);
		file_name = file_info.fileName().mid(0, file_info.fileName().lastIndexOf('.'));
	}

	NoteTitleButton* bar_button = createNoteBarButton(file_name);

	QString file_content;
	if (!getFileContent(file_full_path, file_content))
		return false;

	NoteFileInfo file_info_struct;
	file_info_struct.file_name = file_name;
	file_info_struct.file_name_temp = file_info_struct.file_name;
	file_info_struct.file_content = file_content;
	file_info_struct.file_content_temp = file_info_struct.file_content;
	file_info_struct.note_button = bar_button;

	note_list_.emplace_back(file_info_struct);
	bar_widget_layout_->addWidget(bar_button);
	int index = note_list_.size() - 1;
	bar_group_->addButton(bar_button, index);

	bar_button->setCheckable(true);
	bar_button->setProperty("note_index", index);
	bar_button->setVisible(true);

	connect(bar_button, &NoteTitleButton::pressed, this, &StickyNoteTabWidget::onNoteBarClicked);

	return index;
}

int StickyNoteTabWidget::getNoteBarSenderIndex()
{
	QWidget* bar_widget = qobject_cast<QWidget*>(sender());
	Q_ASSERT(bar_widget);

	return bar_widget->property("note_index").toInt();
}

void StickyNoteTabWidget::onNoteBarClicked()
{
	int index = getNoteBarSenderIndex();
	checkedNote(index);
	content_edit_->setFont(QFont("Microsoft JhengHei", 12));
	return;
}

bool StickyNoteTabWidget::checkedNote(int index)
{
	if (index < 0 || index >= note_list_.size())
	{
		note_content_widget_->setVisible(false);
		return true;
	}

	note_content_widget_->setVisible(true);
	note_list_[index].note_button->setChecked(true);

	auto a = note_list_[index].file_name_temp;
	auto b = note_list_[index].file_content_temp;
	title_name_edit_->setText(a);
	adjustSizetTitleNameEdit();
	content_edit_->setPlainText(b);
	changeSaveStatues(index);
	return true;
}

bool StickyNoteTabWidget::getFileContent(const QString& file_full_path, QString& file_content)
{
	QFile file(file_full_path);

	if (!file.exists())
	{
		file_content.clear();
		return true;
	}

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		LOG(ERROR) << "getFileContent failed,open file failed: " << file_full_path.toStdString();
		return false;
	}

	QTextCodec* codec = QTextCodec::codecForName("GBK");
	file_content = codec->toUnicode(file.readAll());
	file.close();

	return true;
}

bool StickyNoteTabWidget::confirmFolder(const QString& path)
{
	QDir dir;
	if (!dir.exists(path))
	{
		if (!dir.mkdir(path))
			return false;
	}

	return true;
}

NoteTitleButton* StickyNoteTabWidget::createNoteBarButton(const QString& file_name)
{
	NoteTitleButton* note_bar_button = new NoteTitleButton();
	note_bar_button->setWhatsThis("note_bar_button");
	note_bar_button->setFixedHeight(60);
	//note_bar_button->setFixedWidth(200);

	connect(note_bar_button, &NoteTitleButton::rightPressed, this, &StickyNoteTabWidget::onNoteBarRightPressed);

	note_bar_button->setText(QString::fromLocal8Bit(string_utils::trim(file_name.toLocal8Bit().data()).c_str()));

	return note_bar_button;
}

void StickyNoteTabWidget::swapNoteBarButton(int index1, int index2)
{
	auto& note_previous = note_list_[index2];
	auto& note = note_list_[index1];

	NoteFileInfo note_temp;

	note_temp.swip_info(note_previous);
	note_previous.swip_info(note);
	note.swip_info(note_temp);

	QString title_temp = note_previous.note_button->text();
	note_previous.note_button->setText(QString::fromLocal8Bit(string_utils::trim(note.note_button->text().toLocal8Bit().data()).c_str()));
	note.note_button->setText(QString::fromLocal8Bit(string_utils::trim(title_temp.toLocal8Bit().data()).c_str()));

	if (note.note_button->isChecked())
		note_previous.note_button->setChecked(true);
	else if (note_previous.note_button->isChecked())
		note.note_button->setChecked(true);

	saveNoteFileList();
}

void StickyNoteTabWidget::onNoteBarMoveUpClicked()
{
	note_bar_right_pressed_menu_->hide();
	int index = note_bar_right_pressed_menu_->property("note_index").toInt();
	int previous_index = 0;
	if (index > 0)
		swapNoteBarButton(index, index - 1);
}

void StickyNoteTabWidget::onNoteBarMoveDownClicked()
{
	note_bar_right_pressed_menu_->hide();
	int index = note_bar_right_pressed_menu_->property("note_index").toInt();
	int next_index = 0;
	if (index < note_list_.size() - 1);
	swapNoteBarButton(index, index + 1);
}

void StickyNoteTabWidget::onNoteBarMoveDeleteClicked()
{
	note_bar_right_pressed_menu_->hide();
	int index = note_bar_right_pressed_menu_->property("note_index").toInt();
	Q_ASSERT(index < note_list_.size());

	bool is_title_empty = path_utils::isUnduplicatedFolderName(STRING_UNTITLED_NOTES.toLocal8Bit().data(),
		note_list_[index].file_name_temp.toLocal8Bit().data());
	bool is_content_empty = note_list_[index].file_content_temp.isEmpty();
	if (!is_title_empty || !is_content_empty)
	{
		bool is_delete = MessageBox_By(parent_widget_, true, u8"确定删除?", u8"删除后将彻底删除。", u8"删除", u8"取消");
		if (!is_delete)
			return;
	}

	if (bar_group_->checkedId() == index)
	{
		if (note_list_.size() > 0)
			checkedNote(0);
		else
			checkedNote(-1);
	}

	deleteNoteFile(getNoteFileFullPath(note_list_[index].file_name));

	deleteNote(index);
}

void StickyNoteTabWidget::deleteNote(int note_index)
{
	bar_group_->removeButton(note_list_[note_index].note_button);
	note_list_[note_index].note_button->setVisible(false);
	note_list_.erase(note_list_.begin() + note_index);
	for (int i = note_index; i < note_list_.size(); ++i)
	{
		note_list_[i].note_button->setProperty("note_index", i);
		bar_group_->setId(note_list_[i].note_button,i);
	}
	
	return;
}

bool StickyNoteTabWidget::deleteNoteFile(const QString& file_path)
{
	QString file_temp_path = file_path + ".del";
	if (QFile::exists(file_path) && !QFile::rename(file_path, file_temp_path))
	{
		QFile::remove(file_temp_path);

		QFile::rename(file_path, file_temp_path);
		//QFile::remove(file_temp_path);
	}

	return !QFile::exists(file_path);
}

void StickyNoteTabWidget::onNoteBarRightPressed(QPoint pos, int index)
{
	note_bar_right_pressed_menu_->refreshMenu();
	note_bar_right_pressed_menu_->move(pos);

	//为了知道右键的是哪个控件，先用这个笨方法
	note_bar_right_pressed_menu_->setProperty("note_index", index);

	bool show_move_up_button = true;
	bool show_move_down_button = true;

	if (index == 0)
		show_move_up_button = false;
	if (index == note_list_.size() - 1)
		show_move_down_button = false;

	note_bar_right_pressed_menu_->updateMenu(show_move_up_button, show_move_down_button);
	note_bar_right_pressed_menu_->show();
	note_bar_right_pressed_menu_->adjustSize();
}

QPushButton* StickyNoteTabWidget::createNoteSaveTipsButton()
{
	QPushButton* note_save_tips_button = new QPushButton();
	note_save_tips_button->setWhatsThis("note_save_tips_button");

	QHBoxLayout* note_save_tips_layout = new QHBoxLayout();
	note_save_tips_layout->setContentsMargins(0, 6, 0, 0);
	note_save_tips_layout->setSpacing(0);

	note_save_tips_image_saved_ = new QLabel();
	note_save_tips_image_saved_->setWhatsThis("note_save_tips_image");

	QPixmap image_saved(":/public/already_saved");
	QSize image_size(16, 16);
	image_saved = image_saved.scaled(image_size, Qt::KeepAspectRatio);

	note_save_tips_image_saved_->setPixmap(image_saved);

	QPixmap image_dong_save(":/public/doing_save");
	image_dong_save = image_dong_save.scaled(image_size, Qt::KeepAspectRatio);

	note_save_tips_image_doing_save_ = new CRotateWidgetBase();
	note_save_tips_image_doing_save_->setFixedSize(image_size);
	note_save_tips_image_doing_save_->AddPixmap(image_dong_save);
	note_save_tips_image_doing_save_->setVisible(false);

	note_save_tips_content_ = new QLabel();
	note_save_tips_content_->setText(u8"已保存");
	note_save_tips_content_->setWhatsThis("note_save_tips_content");

	note_save_tips_layout->addWidget(note_save_tips_image_saved_);
	note_save_tips_layout->addWidget(note_save_tips_image_doing_save_);

	note_save_tips_layout->addWidget(note_save_tips_content_);
	note_save_tips_layout->addStretch();

	note_save_tips_button->setLayout(note_save_tips_layout);

	return note_save_tips_button;
}

void StickyNoteTabWidget::timerEvent(QTimerEvent* event)
{
	if (event->timerId() == auto_asve_timer_id_&&!auto_asve_timer_pausing)
	{
		saveAllNote();
	}
	QWidget::timerEvent(event);
}

void StickyNoteTabWidget::startAutoSaveTimer()
{
	if (auto_asve_timer_id_ == 0)
	{
		auto_asve_timer_id_ = this->startTimer(auto_asve_interval_, Qt::PreciseTimer);
	}
}