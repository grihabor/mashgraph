#include "../headers/pluginscontainer.h"
#include "../../shared/headers/pluginfilter.h"
#include "../headers/mainwindow.h"

#include <QPushButton>
#include <QComboBox>

PluginsContainer::PluginsContainer(Model* m, QBoxLayout* l, QProgressBar* pb, Logger& logger, MainWindow* window)
    :   imageHoldersLayout(l), progressBar(pb), model(m), Log(logger),
      comboBox(nullptr), applyButton(nullptr), paramsLabel(nullptr), param1(nullptr), mainwindow(window)
{
    QObject::connect(model, SIGNAL(FilterApplied(PluginFilter*, QImage*)), this, SLOT(FilterApplied(PluginFilter*, QImage*)));
}

PluginsContainer::~PluginsContainer()
{
    CleanUp();
}

void PluginsContainer::CleanUp()
{
    for(ImageHolder* h : imageHolders){
        imageHoldersLayout->removeWidget(h);
        delete h;
    }
    if(comboBox){
        removeWidget(comboBox);
        delete comboBox;
        comboBox = nullptr;
    }
    if(applyButton){
        removeWidget(applyButton);
        delete applyButton;
        applyButton = nullptr;
    }
    if(paramsLabel){
        removeWidget(paramsLabel);
        delete paramsLabel;
        paramsLabel = nullptr;
    }
    if(param1){
        removeWidget(param1);
        delete param1;
        param1 = nullptr;
    }
    imageHolders.clear();
}

void PluginsContainer::ClearFilters()
{
    Log("Clear image views");
    for(ImageHolder* h : imageHolders){
        imageHoldersLayout->removeWidget(h);
        delete h;
    }
    imageHolders.clear();
}

void PluginsContainer::ShowPlugins(QVector<PluginFilter *> pluginVector)
{
    Log("PluginsContainer: cleanup");
    CleanUp();

    if(pluginVector.size() == 0){
        Log("No plugins to show");
        return;
    }

    Log("Show plugins list");

    applyButton = new QPushButton;
    applyButton->setText("Apply filter");

    comboBox = new QComboBox;
    for(PluginFilter* plugin : pluginVector){
        Log(QString("Add \'") + plugin->Name().c_str() + "\' item");
        comboBox->addItem(plugin->Name().c_str());
    }
    QObject::connect(applyButton, SIGNAL(clicked()), this, SLOT(ApplyButtonClicked()));
    QObject::connect(applyButton, SIGNAL(clicked()), mainwindow, SLOT(BlockButtons()));

    paramsLabel = new QLabel;
    paramsLabel->setText("Params: ");
    paramsLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    param1 = new QDoubleSpinBox;
    param1->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

    addWidget(comboBox);
    addWidget(paramsLabel);
    addWidget(param1);
    addWidget(applyButton);
}

void PluginsContainer::BlockButtons(bool b)
{
    if(applyButton)
        applyButton->blockSignals(b);
}

void PluginsContainer::ApplyButtonClicked()
{
    int index = comboBox->currentIndex();
    Log("Apply filter " + QString::number(index));
    progressBar->show();

    params[0] = param1->value();

    Log("Apply filter " + QString::number(index)
        + " with params: " + QString::number(params[0]));
    model->ApplyFilter(index, params);
}

void PluginsContainer::FilterApplied(PluginFilter* filter, QImage *img)
{
    ImageHolder *h = new ImageHolder(filter->Name(), progressBar, Log);
    imageHoldersLayout->insertWidget(0, h);
    imageHoldersLayout->setAlignment(h, Qt::AlignLeft | Qt::AlignTop);
    h->HandleSignal(img);
    imageHolders.push_back(h);
}

void PluginsContainer::RemoveLastFilter()
{
    ImageHolder* lastFilter = imageHolders[imageHolders.size()-1];
    imageHoldersLayout->removeWidget(lastFilter);
    delete lastFilter;
    imageHolders.pop_back();
}




