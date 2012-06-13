/*
	Aseba - an event-based framework for distributed robot control
	Copyright (C) 2007--2012:
		Stephane Magnenat <stephane at magnenat dot net>
		(http://stephane.magnenat.net)
		and other contributors, see authors.txt for details
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published
	by the Free Software Foundation, version 3 of the License.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ConfigDialog.h"

#include <QSettings>

#include <ConfigDialog.moc>

namespace Aseba
{
	/*** ConfigPage ***/
	ConfigPage::ConfigPage(QString title, QWidget *parent):
		QWidget(parent)
	{
		QLabel* myTitle = new QLabel(title);

		mainLayout = new QVBoxLayout();
		mainLayout->addWidget(myTitle);
		setLayout(mainLayout);
	}


	/*** GeneralPage ***/
	GeneralPage::GeneralPage(QWidget *parent):
		ConfigPage(tr("General Setup"), parent)
	{
		mainLayout->addStretch();
	}

	void GeneralPage::readSettings()
	{
		QSettings settings;
		settings.beginGroup("general-config");
		settings.endGroup();
	}

	void GeneralPage::writeSettings()
	{
		QSettings settings;
		settings.beginGroup("general-config");
		settings.endGroup();
	}


	/*** EditorPage ***/
	EditorPage::EditorPage(QWidget *parent):
		ConfigPage(tr("Editor Setup"), parent)
	{
		autocompletion = new QCheckBox(tr("Enable autocompletion"));
		mainLayout->addWidget(autocompletion);
		mainLayout->addStretch();
	}

	void EditorPage::readSettings()
	{
		QSettings settings;
		settings.beginGroup("editor-config");
		autocompletion->setCheckState(settings.value("autocompletion", true).toBool() == true ? Qt::Checked : Qt::Unchecked);
		settings.endGroup();
	}

	void EditorPage::writeSettings()
	{
		QSettings settings;
		settings.beginGroup("editor-config");
		settings.setValue("autocompletion", autocompletion->checkState() == Qt::Checked);
		settings.endGroup();
	}


	/*** ConfigDialog ***/
	void ConfigDialog::init(QWidget* parent)
	{
		if (!me)
			me = new ConfigDialog(parent);
	}

	void ConfigDialog::bye()
	{
		if (me)
		{
			delete me;
			me = NULL;
		}
	}

	void ConfigDialog::showConfig()
	{
		me->setModal(true);
		me->show();

	}

	ConfigDialog* ConfigDialog::me = NULL;

	ConfigDialog::ConfigDialog(QWidget* parent):
		QDialog(parent)
	{
		// list of topics
		topicList = new QListWidget();
		topicList->addItem("General");
		topicList->addItem("Editor");

		// create pages
		configStack = new QStackedWidget();
		generalpage = new GeneralPage();
		configStack->addWidget(generalpage);
		editorpage = new EditorPage();
		configStack->addWidget(editorpage);

		connect(topicList, SIGNAL(currentRowChanged(int)), configStack, SLOT(setCurrentIndex(int)));
		topicList->setCurrentRow(0);

		// buttons
		okButton = new QPushButton(tr("Ok"));
		cancelButton = new QPushButton(tr("Cancel"));
		connect(okButton, SIGNAL(clicked()), SLOT(accept()));
		connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));

		// main layout
		QHBoxLayout* contentLayout = new QHBoxLayout();
		contentLayout->addWidget(topicList);
		contentLayout->addWidget(configStack);

		QHBoxLayout* buttonLayout = new QHBoxLayout();
		buttonLayout->addStretch();
		buttonLayout->addWidget(okButton);
		buttonLayout->addWidget(cancelButton);

		QVBoxLayout* mainLayout = new QVBoxLayout();
		mainLayout->addLayout(contentLayout);
		mainLayout->addLayout(buttonLayout);

		setLayout(mainLayout);

		readSettings();
	}

	ConfigDialog::~ConfigDialog()
	{
		writeSettings();
	}

	bool ConfigDialog::getAutoCompletion()
	{
		if (me)
			return (me->editorpage->autocompletion->checkState() == Qt::Unchecked ? false : true);
		else
			return false;
	}

	void ConfigDialog::accept()
	{
		QDialog::accept();
	}

	void ConfigDialog::reject()
	{
		QDialog::reject();
	}

	void ConfigDialog::readSettings()
	{
		for (int i = 0; i < configStack->count(); i++)
		{
			ConfigPage* config = dynamic_cast<ConfigPage*>(configStack->widget(i));
			if (config)
				config->readSettings();
		}
	}

	void ConfigDialog::writeSettings()
	{
		for (int i = 0; i < configStack->count(); i++)
		{
			ConfigPage* config = dynamic_cast<ConfigPage*>(configStack->widget(i));
			if (config)
				config->writeSettings();
		}
	}

}

