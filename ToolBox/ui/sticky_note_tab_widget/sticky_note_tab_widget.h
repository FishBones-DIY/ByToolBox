#pragma once

#include <list>
#include <vector>
#include <utility>


#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QPlainTextEdit>
#include <QStackedWidget>

#include "by_line_edit.h"
#include "by_plain_text_edit.h"

#include "note_title_button.h"
#include "note_title_right_press_menu.h"

#include "ui/rotate/CRotateWidgetBase.h"

struct NoteFileInfo
{
	QPushButton* note_button;
	QString file_name;
	QString file_name_temp;
	QString file_content;
	QString file_content_temp;

	NoteFileInfo& swip_info(NoteFileInfo other)
	{
		file_name = other.file_name;
		file_name_temp = other.file_name_temp;
		file_content = other.file_content;
		file_content_temp = other.file_content_temp;
		return *this;
	}
};

class StickyNoteTabWidget : public QWidget
{
	Q_OBJECT
public:
	StickyNoteTabWidget(QWidget* parent = Q_NULLPTR);
public:
	int addNote(const QString& file_full_path);
	bool confirmFolder(const QString& path);
	bool getFileContent(const QString& file_full_path, QString& file_content);
	bool checkedNote(int index);
	void changeSaveImage(bool saved);

public slots:
	void onNoteBarClicked();
	void onNoteSaveButtonClicked();
	void onNoteTextChanged();
	void onNoteTitleChanged();
	void onNoteAddButtonClicked();
	void onNoteTitleEditFocusIn();
	void onNoteTitleEditFocusOut();

	void onNoteBarMoveUpClicked();
	void onNoteBarMoveDownClicked();
	void onNoteBarMoveDeleteClicked();

	void onNoteBarRightPressed(QPoint,int);

protected:
	virtual void timerEvent(QTimerEvent* event) override;
private:
	void loadLocalNotes();
	void swapNoteBarButton(int index1,int index2);
	bool writeNote(int note_index);
	bool writeFile(QString file_full_path,QString file_content);
	bool changeNoteFileName(QString& old_name, const QString& new_name);
	QString getNotePathFullPath(const QString& file_name);
	QString getNoteFileFullPath(const QString& file_name);
	QWidget* createNoteBarWidget();
	
	QWidget* createNoteContentWidger();
	NoteTitleButton* createNoteBarButton(const QString& file_name);
	QPushButton* createNoteSaveTipsButton();
	int getTitleNameEditTextWidth();
	void adjustSizetTitleNameEdit();
	bool isContentChanged(int note_index);
	bool isTitleChanged(int note_index);
	void changeSaveStatues(int note_index);
	std::vector<QString> getNoteFileList();
	bool saveNoteFileList();
	int getNoteBarSenderIndex();
	void deleteNote(int note_index);
	void createNoteBarRightPressedMenu();

	bool deleteNoteFile(const QString& file_path);
	void startAutoSaveTimer();
	void saveNote(int note_index);
	void saveAllNote();
private:
	QWidget* parent_widget_{ nullptr };
	QString note_file_save_dir_;
	QVBoxLayout* bar_widget_layout_{ nullptr };
	QButtonGroup* bar_group_{ nullptr };
	ByLineEdit* title_name_edit_{ nullptr };
	ByTextEdit* content_edit_{ nullptr };
	QPushButton* note_save_button_{ nullptr };
	QPushButton* note_save_tips_button_{ nullptr };
	QLabel* note_save_tips_image_saved_{ nullptr };
	QLabel* note_save_tips_content_{ nullptr };
	CRotateWidgetBase* note_save_tips_image_doing_save_{ nullptr };
	NoteTitleRightPressMenu* note_bar_right_pressed_menu_{ nullptr };
	QWidget* note_content_widget_{ nullptr };

	int note_title_name_edit_max_width_{ 500 };

	bool is_edit_mode_{ false };

	std::vector<NoteFileInfo> note_list_;

	bool auto_asve_timer_pausing{ false };
	int auto_asve_timer_id_{0}; // 自动保存定时器id.
	int auto_asve_interval_{ 1000 * 5 }; // 自动保存间隔5s

private:
	QString STRING_UNTITLED_NOTES{ u8"无标题笔记" };
};