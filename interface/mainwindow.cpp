#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    renderWidget = new RenderWidget(this);
    connect(renderWidget, SIGNAL(cameraXChanged(double)), ui->cameraXSpinBox, SLOT(setValue(double)));
    connect(renderWidget, SIGNAL(cameraYChanged(double)), ui->cameraYSpinBox, SLOT(setValue(double)));
    connect(renderWidget, SIGNAL(cameraZChanged(double)), ui->cameraZSpinBox, SLOT(setValue(double)));
    connect(renderWidget, SIGNAL(yawChanged(double)), this, SLOT(updateYaw(double)));
    connect(renderWidget, SIGNAL(pitchChanged(double)), this, SLOT(updatePitch(double)));
    connect(this, SIGNAL(cameraParamChanged(CameraChange)), renderWidget, SLOT(changeCamera(CameraChange)));
    connect(this, SIGNAL(modelParamChanged(ModelChange)), renderWidget, SLOT(changeModel(ModelChange)));
    connect(this, SIGNAL(modelMaterialChanged(Material)), renderWidget, SLOT(changeMaterial(Material)));
    connect(this, SIGNAL(lightColorChanged(QColor)), renderWidget, SLOT(changeLight(QColor)));
    connect(this, SIGNAL(lightPositionChanged(double, double, double)), renderWidget, SLOT(moveLight(double, double, double)));
    connect(renderWidget, SIGNAL(catchedNothing()), this, SLOT(switchCreatePage()));
    connect(renderWidget, SIGNAL(catchedModel(ModelAttributes, Material)), this, SLOT(switchConfigurePage(ModelAttributes, Material)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pitchSlider_actionTriggered(int action)
{
    int position = ui->pitchSlider->sliderPosition();
    ui->pitchSpinBox->setValue(position);
    CameraChange change{CameraParam::PITCH, static_cast<double>(position)};
    emit cameraParamChanged(change);
    Q_UNUSED(action)
}

void MainWindow::on_yawSlider_actionTriggered(int action)
{
    int position = ui->yawSlider->sliderPosition();
    ui->yawSpinBox->setValue(position);
    CameraChange change{CameraParam::YAW, static_cast<double>(position)};
    emit cameraParamChanged(change);
    Q_UNUSED(action)
}

void MainWindow::on_pitchSpinBox_editingFinished()
{
    ui->pitchSlider->setValue(static_cast<int>(ui->pitchSpinBox->value()));
    CameraChange change{CameraParam::PITCH, ui->pitchSpinBox->value()};
    emit cameraParamChanged(change);
}


void MainWindow::on_yawSpinBox_editingFinished()
{
    ui->yawSlider->setValue(static_cast<int>(ui->yawSpinBox->value()));
    CameraChange change{CameraParam::YAW, ui->yawSpinBox->value()};
    emit cameraParamChanged(change);
}

void MainWindow::updateYaw(double yaw)
{
    ui->yawSlider->setValue(static_cast<int>(yaw));
    ui->yawSpinBox->setValue(yaw);
}

void MainWindow::updatePitch(double pitch)
{
    ui->pitchSlider->setValue(static_cast<int>(pitch));
    ui->pitchSpinBox->setValue(pitch);
}


void MainWindow::on_cameraXSpinBox_editingFinished()
{
    CameraChange change{CameraParam::POS_X, ui->cameraXSpinBox->value()};
    emit cameraParamChanged(change);
}

void MainWindow::on_cameraYSpinBox_editingFinished()
{
    CameraChange change{CameraParam::POS_Y, ui->cameraYSpinBox->value()};
    emit cameraParamChanged(change);
}

void MainWindow::on_cameraZSpinBox_editingFinished()
{
    CameraChange change{CameraParam::POS_Z, ui->cameraZSpinBox->value()};
    emit cameraParamChanged(change);
}

void MainWindow::on_nearPlaneSpinBox_editingFinished()
{
    CameraChange change{CameraParam::NEAR_PLANE, ui->nearPlaneSpinBox->value()};
    emit cameraParamChanged(change);
}

void MainWindow::on_farPlaneSpinBox_editingFinished()
{
    CameraChange change{CameraParam::FAR_PLANE, ui->farPlaneSpinBox->value()};
    emit cameraParamChanged(change);
}

void MainWindow::on_fovSpinBox_editingFinished()
{
    CameraChange change{CameraParam::FOV, ui->fovSpinBox->value()};
    emit cameraParamChanged(change);
}

void MainWindow::on_sensSpinBox_editingFinished()
{
    CameraChange change{CameraParam::SENSITIVITY, ui->sensSpinBox->value()};
    emit cameraParamChanged(change);
}

void MainWindow::on_createButton_clicked()
{
    ModelAttributes attributes;
    Material material;
    attributes.lengthTop = ui->createTopLengthSpinBox->value();
    attributes.lengthBot = ui->createBotLengthSpinBox->value();
    attributes.height = ui->createHeightSpinBox->value();
    attributes.nVerts = ui->createVertsSpinBox->value();
    material.shininess_ = ui->createShineSpinBox->value();
    double specular = ui->createSpecularSpinBox->value();
    material.specular_ = Color(specular, specular, specular);
    QColor color = ui->createModelFrame->palette().color(QWidget::backgroundRole());
    material.diffuse_ = Color(color.redF(), color.greenF(), color.blueF());
    int nModels = renderWidget->addModel(attributes, material);
//    if (nModels == MAX_MODELS)
//    {
//        ui->createButton->setEnabled(false);
//    }
}

void MainWindow::switchCreatePage()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::switchConfigurePage(ModelAttributes attributes, Material material)
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->changeTopLengthSpinBox->setValue(attributes.lengthTop);
    ui->changeBotLengthSpinBox->setValue(attributes.lengthBot);
    ui->changeHeightSpinBox->setValue(attributes.height);
    ui->changeVertsSpinBox->setValue(attributes.nVerts);
    ui->changeShineSpinBox->setValue(material.shininess_);
    ui->changeSpecularSpinBox->setValue(material.specular_.r());
    QPalette palette;
    QColor color (material.diffuse_.rgb());
    palette.setColor(QPalette::Background, color);
    ui->changeModelFrame->setPalette(palette);
    ui->changeModelFrame->setStyleSheet(QString("QFrame { background-color: rgb(%1, %2, %3);}").arg(color.red()).arg(color.green()).arg(color.blue()));
}

void MainWindow::on_changeTopLengthSpinBox_editingFinished()
{
    ModelChange change{ModelParam::TOP, ui->changeTopLengthSpinBox->value()};
    emit modelParamChanged(change);
}

void MainWindow::on_changeBotLengthSpinBox_editingFinished()
{
    ModelChange change{ModelParam::BOT, ui->changeBotLengthSpinBox->value()};
    emit modelParamChanged(change);
}

void MainWindow::on_changeHeightSpinBox_editingFinished()
{
    ModelChange change{ModelParam::HEIGHT, ui->changeHeightSpinBox->value()};
    emit modelParamChanged(change);
}

void MainWindow::on_changeVertsSpinBox_editingFinished()
{
    ModelChange change{ModelParam::VERTS, static_cast<double>(ui->changeVertsSpinBox->value())};
    emit modelParamChanged(change);
}

void MainWindow::on_deleteModelButton_clicked()
{
    renderWidget->deleteModel();
    ui->stackedWidget->setCurrentIndex(1);
    ui->createButton->setEnabled(true);
}

void MainWindow::on_rotateXButton_clicked()
{
    ModelMovement move{ui->angleXSpinBox->value(), 0, 0, 0, 0, 0};
    renderWidget->moveModel(move);
}

void MainWindow::on_rotateYButton_clicked()
{
    ModelMovement move{0, ui->angleYSpinBox->value(), 0, 0, 0, 0};
    renderWidget->moveModel(move);
}

void MainWindow::on_rotateZButton_clicked()
{
    ModelMovement move{0, 0, ui->angleZSpinBox->value(), 0, 0, 0};
    renderWidget->moveModel(move);
}

void MainWindow::on_transferButton_clicked()
{
    ModelMovement move{0, 0, 0, ui->dxSpinBox->value(), ui->dySpinBox->value(), ui->dzSpinBox->value()};
    renderWidget->moveModel(move);
}

void MainWindow::on_clearSceneButton_clicked()
{
    renderWidget->deleteAllModels();
    ui->createButton->setEnabled(true);
}

void MainWindow::on_chooseLightColorButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this,"Выберите цвет");
    if (color.isValid())
    {
        QPalette palette;
        palette.setColor(QPalette::Background, color);
        ui->LightFrame->setPalette(palette);
        emit lightColorChanged(color);
    }
}

void MainWindow::on_changeModelColorButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this,"Выберите цвет");
    if (color.isValid())
    {
        QPalette palette;
        palette.setColor(QPalette::Background, color);
        ui->changeModelFrame->setPalette(palette);
        ui->changeModelFrame->setStyleSheet(QString("QFrame { background-color: rgb(%1, %2, %3);}").arg(color.red()).arg(color.green()).arg(color.blue()));
        Material material;
        material.shininess_ = ui->changeShineSpinBox->value();
        double specular = ui->changeSpecularSpinBox->value();
        material.specular_ = Color(specular, specular, specular);
        material.diffuse_ = Color(color.redF(), color.greenF(), color.blueF());
        emit modelMaterialChanged(material);
    }
}

void MainWindow::on_createModelColorButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this,"Выберите цвет");
    if (color.isValid())
    {
        QPalette palette;
        palette.setColor(QPalette::Background, color);
        ui->createModelFrame->setPalette(palette);
        ui->createModelFrame->setStyleSheet(QString("QFrame { background-color: rgb(%1, %2, %3);}").arg(color.red()).arg(color.green()).arg(color.blue()));
    }
}

void MainWindow::on_changeSpecularSpinBox_editingFinished()
{
    Material material;
    QColor color = ui->changeModelFrame->palette().color(QWidget::backgroundRole());
    material.shininess_ = ui->changeShineSpinBox->value();
    double specular = ui->changeSpecularSpinBox->value();
    material.specular_ = Color(specular, specular, specular);
    material.diffuse_ = Color(color.redF(), color.greenF(), color.blueF());
    emit modelMaterialChanged(material);
}

void MainWindow::on_changeShineSpinBox_editingFinished()
{
    Material material;
    QColor color = ui->changeModelFrame->palette().color(QWidget::backgroundRole());
    material.shininess_ = ui->changeShineSpinBox->value();
    double specular = ui->changeSpecularSpinBox->value();
    material.specular_ = Color(specular, specular, specular);
    material.diffuse_ = Color(color.redF(), color.greenF(), color.blueF());
    emit modelMaterialChanged(material);
}

void MainWindow::on_lightXSpinBox_editingFinished()
{
    emit lightPositionChanged(ui->lightXSpinBox->value(), ui->lightYSpinBox->value(), ui->lightZSpinBox->value());
}

void MainWindow::on_lightYSpinBox_editingFinished()
{
    emit lightPositionChanged(ui->lightXSpinBox->value(), ui->lightYSpinBox->value(), ui->lightZSpinBox->value());
}


void MainWindow::on_lightZSpinBox_editingFinished()
{
    emit lightPositionChanged(ui->lightXSpinBox->value(), ui->lightYSpinBox->value(), ui->lightZSpinBox->value());
}

void MainWindow::on_createSceneButton1_clicked()
{
    renderWidget->deleteAllModels();
    //ui->createButton->setEnabled(false);
    ModelAttributes attributes;
    Material material;
    attributes.lengthTop = 0;
    attributes.lengthBot = 50;
    attributes.height = 300;
    attributes.nVerts = 6;
    material.shininess_ = 256;
    material.specular_ = Color(1.0f, 1.0f, 1.0f);
    QColor color(64, 255, 0);
    material.diffuse_ = Color(color.redF(), color.greenF(), color.blueF());
    renderWidget->addModel(attributes, material);
    attributes.lengthTop = 100;
    attributes.lengthBot = 100;
    attributes.height = 100;
    attributes.nVerts = 4;
    material.shininess_ = 128;
    material.specular_ = Color(1.0f, 1.0f, 1.0f);
    color = QColor(255, 10, 0);
    material.diffuse_ = Color(color.redF(), color.greenF(), color.blueF());
    renderWidget->addModel(attributes, material);
    attributes.lengthTop = 70;
    attributes.lengthBot = 10;
    attributes.height = 180;
    attributes.nVerts = 10;
    material.shininess_ = 512;
    material.specular_ = Color(1.0f, 1.0f, 1.0f);
    color = QColor(0, 0, 255);
    material.diffuse_ = Color(color.redF(), color.greenF(), color.blueF());
    renderWidget->addModel(attributes, material);
    renderWidget->setModel(2);
    ModelMovement move = {0, 0, -135, -300, 0, 300};
    renderWidget->moveModel(move);
    ui->cameraXSpinBox->setValue(-1);
    ui->cameraYSpinBox->setValue(268);
    ui->cameraZSpinBox->setValue(434);
    CameraChange change{CameraParam::POS_X, -1};
    emit cameraParamChanged(change);
    change = {CameraParam::POS_Y, 268};
    emit cameraParamChanged(change);
    change = {CameraParam::POS_Z, 434};
    emit cameraParamChanged(change);
    change = {CameraParam::PITCH, -32};
    emit cameraParamChanged(change);
    change = {CameraParam::YAW, -114};
    emit cameraParamChanged(change);
    updateYaw(-114);
    updatePitch(-32);
    ui->lightXSpinBox->setValue(-100);
    ui->lightYSpinBox->setValue(110);
    ui->lightZSpinBox->setValue(170);
    emit lightPositionChanged(-100, 110, 170);
    color = {255, 255, 255};
    QPalette palette;
    palette.setColor(QPalette::Background, color);
    ui->LightFrame->setPalette(palette);
    emit lightColorChanged(color);
}

void MainWindow::on_createSceneButton2_clicked()
{
    renderWidget->deleteAllModels();
    //ui->createButton->setEnabled(false);
    ModelAttributes attributes;
    Material material;
    attributes.lengthTop = 100;
    attributes.lengthBot = 100;
    attributes.height = 150;
    attributes.nVerts = 30;
    material.shininess_ = 256;
    material.specular_ = Color(0.0f, 0.0f, 0.0f);
    QColor color(29, 114, 255);
    material.diffuse_ = Color(color.redF(), color.greenF(), color.blueF());
    renderWidget->addModel(attributes, material);
    renderWidget->setModel(0);
    ModelMovement move{0, 0, 0, 0, 0, -400};
    renderWidget->moveModel(move);
    attributes.lengthTop = 40;
    attributes.lengthBot = 40;
    attributes.height = 50;
    attributes.nVerts = 6;
    material.shininess_ = 512;
    material.specular_ = Color(1.0f, 1.0f, 1.0f);
    color = QColor(255, 248, 0);
    material.diffuse_ = Color(color.redF(), color.greenF(), color.blueF());
    renderWidget->addModel(attributes, material);
    renderWidget->setModel(1);
    move = {90, 0, 0, 0, 0, -100};
    renderWidget->moveModel(move);
    attributes.lengthTop = 0;
    attributes.lengthBot = 10;
    attributes.height = 30;
    attributes.nVerts = 10;
    material.shininess_ = 512;
    material.specular_ = Color(1.0f, 1.0f, 1.0f);
    color = QColor(255, 76, 0);
    material.diffuse_ = Color(color.redF(), color.greenF(), color.blueF());
    renderWidget->addModel(attributes, material);
    ui->cameraXSpinBox->setValue(108);
    ui->cameraYSpinBox->setValue(49);
    ui->cameraZSpinBox->setValue(280);
    CameraChange change{CameraParam::POS_X, 108};
    emit cameraParamChanged(change);
    change = {CameraParam::POS_Y, 49};
    emit cameraParamChanged(change);
    change = {CameraParam::POS_Z, 280};
    emit cameraParamChanged(change);
    change = {CameraParam::PITCH, -11};
    emit cameraParamChanged(change);
    change = {CameraParam::YAW, -115};
    emit cameraParamChanged(change);
    updateYaw(-115);
    updatePitch(-11);
    ui->lightXSpinBox->setValue(0);
    ui->lightYSpinBox->setValue(0);
    ui->lightZSpinBox->setValue(250);
    emit lightPositionChanged(0, 0, 250);
    color = {255, 255, 255};
    QPalette palette;
    palette.setColor(QPalette::Background, color);
    ui->LightFrame->setPalette(palette);
    emit lightColorChanged(color);
}

void MainWindow::on_createSceneButton3_clicked()
{
    renderWidget->deleteAllModels();
    //ui->createButton->setEnabled(false);
    ModelAttributes attributes;
    Material material;
    attributes.lengthTop = 0;
    attributes.lengthBot = 10;
    attributes.height = 25;
    attributes.nVerts = 6;
    material.shininess_ = 512;
    material.specular_ = Color(1.0f, 1.0f, 1.0f);
    QColor color(33, 255, 6);
    material.diffuse_ = Color(color.redF(), color.greenF(), color.blueF());
    renderWidget->addModel(attributes, material);
    renderWidget->setModel(0);
    ModelMovement move{0, 0, 0, 0, 15, 0};
    renderWidget->moveModel(move);
    attributes.lengthTop = 10;
    attributes.lengthBot = 0;
    attributes.height = 25;
    attributes.nVerts = 6;
    material.shininess_ = 512;
    material.specular_ = Color(1.0f, 1.0f, 1.0f);
    color = QColor(33, 255, 6);
    material.diffuse_ = Color(color.redF(), color.greenF(), color.blueF());
    renderWidget->addModel(attributes, material);
    renderWidget->setModel(1);
    move = {0, 0, 0, 0, -10, 0};
    renderWidget->moveModel(move);
    attributes.lengthTop = 130;
    attributes.lengthBot = 60;
    attributes.height = 170;
    attributes.nVerts = 80;
    material.shininess_ = 512;
    material.specular_ = Color(1.0f, 1.0f, 1.0f);
    color = QColor(255, 255, 255);
    material.diffuse_ = Color(color.redF(), color.greenF(), color.blueF());
    renderWidget->addModel(attributes, material);
    renderWidget->setModel(2);
    move = {-50, 25, 0, 0, -65, -160};
    renderWidget->moveModel(move);
    CameraChange change{CameraParam::POS_X, -27};
    emit cameraParamChanged(change);
    ui->cameraXSpinBox->setValue(-27);
    ui->cameraYSpinBox->setValue(24);
    ui->cameraZSpinBox->setValue(109);
    change = {CameraParam::POS_Y, 24};
    emit cameraParamChanged(change);
    change = {CameraParam::POS_Z, 109};
    emit cameraParamChanged(change);
    change = {CameraParam::PITCH, -2};
    emit cameraParamChanged(change);
    change = {CameraParam::YAW, -64};
    emit cameraParamChanged(change);
    updateYaw(-64);
    updatePitch(-2);
    ui->lightXSpinBox->setValue(20);
    ui->lightYSpinBox->setValue(50);
    ui->lightZSpinBox->setValue(80);
    emit lightPositionChanged(20, 50, 80);
    color = {255, 255, 102};
    QPalette palette;
    palette.setColor(QPalette::Background, color);
    ui->LightFrame->setPalette(palette);
    emit lightColorChanged(color);
}
