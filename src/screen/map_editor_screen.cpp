#include "map_editor_screen.h"
#include "config.h"
#include <QFileDialog>
#include <QDir>
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>

MapEditorScreen::MapEditorScreen(QWidget *parent) : Screen(parent)
{
    setFixedSize(QSize(800, 600));

    // 检查并创建 maps 文件夹
    QDir mapDir(DEFAULT_MAP_DIR);
    if (!mapDir.exists()) {
        if (!mapDir.mkpath(".")) {
            qDebug() << "Failed to create maps directory";
        }
    }

    // map list
    mapListWidget = new QListWidget(this);
    updateMapList();
    connect(mapListWidget, &QListWidget::itemClicked, this, &MapEditorScreen::onMapListItemClicked); // 连接信号和槽

    mapNameEdit = new QLineEdit(this);
    mapNameEdit->setPlaceholderText("输入地图名称");

    saveButton = new QPushButton("保存地图", this);
    connect(saveButton, &QPushButton::clicked, this, &MapEditorScreen::saveMap);

    loadMapButton = new QPushButton("导入地图", this);
    connect(loadMapButton, &QPushButton::clicked, this, &MapEditorScreen::onLoadMapClicked);

    exitButton = new QPushButton("退出", this);
    connect(exitButton, &QPushButton::clicked, [this]{
        emit backToStartScreen();
    });

    editorScene = new QGraphicsScene(this);
    editorView = new QGraphicsView(editorScene);
    editorView->setMouseTracking(true);

    // 安装事件过滤器
    editorView->viewport()->installEventFilter(this);

    // 加载默认地图
    mapEdited = Map::getInstance();
    mapEdited->load(DEFAULT_MAP_PATH);
    drawMap();
    drawGrid();

    // layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(mapListWidget);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(mapNameEdit);
    inputLayout->addWidget(saveButton);
    inputLayout->addWidget(loadMapButton);
    inputLayout->addWidget(exitButton);
    mainLayout->addLayout(inputLayout);

    setupTerrainSelection();

    mainLayout->addWidget(editorView);

    selectedTerrainType = 1;  // 默认选择泥土地形
}

MapEditorScreen::~MapEditorScreen() {}

void MapEditorScreen::updateMapList()
{
    mapListWidget->clear();
    QDir mapDir("maps");
    QStringList mapFiles = mapDir.entryList(QStringList() << "*.txt", QDir::Files);
    for (const QString &file : mapFiles) {
        mapListWidget->addItem(file);
    }
}

void MapEditorScreen::saveMap()
{
    QString mapName = mapNameEdit->text();
    if (mapName.isEmpty()) {
        return;
    }

    QString filePath = "maps/" + mapName + ".txt";
    QFile file(filePath);
    if (file.exists()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "确认覆盖", "该地图文件已存在，是否覆盖？",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }
    }

    mapEdited->save(filePath);

    updateMapList();
}

void MapEditorScreen::onCellClicked(const QPointF& pos)
{
    int x = static_cast<int>(pos.x()) / TERRAIN_WIDTH;
    int y = static_cast<int>(pos.y()) / TERRAIN_HEIGHT;

    // 修正索引判断和访问逻辑
    if (y >= 0 && y < mapEdited->map.size() && x >= 0 && x < (mapEdited->map)[y].size()) {
        delete (mapEdited->map)[y][x];
        switch (selectedTerrainType) {
        case 1:
            (mapEdited->map)[y][x] = new Mud();
            break;
        case 2:
            (mapEdited->map)[y][x] = new Grass();
            break;
        case 3:
            (mapEdited->map)[y][x] = new Ice();
            break;
        default:
            (mapEdited->map)[y][x] = new Null();
            break;
        }
        drawMap();
        drawGrid();
    }
}

void MapEditorScreen::drawMap()
{
    editorScene->clear();
    for (int y = 0; y < mapEdited->map.size(); ++y) {
        for (int x = 0; x < (mapEdited->map)[y].size(); ++x) {
            Terrain* terrain = (mapEdited->map)[y][x];
            if (terrain) {
                if (terrain->getTypeId() != 0) { // 0 代表 Null 地形
                    QString path;
                    switch (terrain->getTypeId()) {
                    case 1:
                        path = ":/img/terrain/mud.png";
                        break;
                    case 2:
                        path = ":/img/terrain/grass.png";
                        break;
                    case 3:
                        path = ":/img/terrain/ice.png";
                        break;
                    default:
                        path = "";
                        break;
                    }
                    if (!path.isEmpty()) {
                        QPixmap pixmap(path);
                        pixmap = pixmap.scaled(TERRAIN_WIDTH, TERRAIN_HEIGHT, Qt::KeepAspectRatio); // 调整图片大小
                        if (!pixmap.isNull()) {
                            editorScene->addPixmap(pixmap)->setPos(x * TERRAIN_WIDTH, y * TERRAIN_HEIGHT);
                        }
                    }
                }
            }
        }
    }
}

void MapEditorScreen::drawGrid()
{
    QPen pen(Qt::black);
    pen.setWidth(1);

    int maxX = mapEdited->getWidth();
    int maxY = mapEdited->getHeight();

    for (int x = 0; x <= maxX * TERRAIN_WIDTH; x += TERRAIN_WIDTH) {
        editorScene->addLine(x, 0, x, maxY * TERRAIN_HEIGHT, pen);
    }

    for (int y = 0; y <= maxY * TERRAIN_HEIGHT; y += TERRAIN_HEIGHT) {
        editorScene->addLine(0, y, maxX * TERRAIN_WIDTH, y, pen);
    }
}

bool MapEditorScreen::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == editorView->viewport() && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        onCellClicked(editorView->mapToScene(mouseEvent->pos()));
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void MapEditorScreen::onTerrainSelected(int id)
{
    selectedTerrainType = id;
}

void MapEditorScreen::setupTerrainSelection()
{
    terrainButtonGroup = new QButtonGroup(this);
    QHBoxLayout *mainLayout = new QHBoxLayout();

    createTerrainButtonAndView("Null", 0, "", terrainButtonGroup, mainLayout);
    createTerrainButtonAndView("Mud", 1, ":/img/terrain/mud.png", terrainButtonGroup, mainLayout);
    createTerrainButtonAndView("Grass", 2, ":/img/terrain/grass.png", terrainButtonGroup, mainLayout);
    createTerrainButtonAndView("Ice", 3, ":/img/terrain/ice.png", terrainButtonGroup, mainLayout);

    // 添加弹簧项以实现水平均布
    mainLayout->addStretch(1);
    mainLayout->insertStretch(0, 1);

    QVBoxLayout *terrainLayout = new QVBoxLayout();
    terrainLayout->addLayout(mainLayout);

    QVBoxLayout *mainWindowLayout = dynamic_cast<QVBoxLayout*>(layout());
    if (mainWindowLayout) {
        mainWindowLayout->addLayout(terrainLayout);
    }
}

void MapEditorScreen::createTerrainButtonAndView(const QString& buttonText, int id, const QString& path, QButtonGroup* buttonGroup, QHBoxLayout* mainLayout)
{
    QVBoxLayout *vLayout = new QVBoxLayout();
    QPushButton *button = new QPushButton(buttonText, this);
    buttonGroup->addButton(button, id);
    connect(buttonGroup, SIGNAL(idClicked(int)), this, SLOT(onTerrainSelected(int)));

    QGraphicsView *view = new QGraphicsView(this);
    QGraphicsScene *scene = new QGraphicsScene(view);
    QPixmap pixmap(path);
    pixmap = pixmap.scaled(50, 50, Qt::KeepAspectRatio);
    scene->addPixmap(pixmap);
    view->setScene(scene);
    view->setFixedSize(50, 50);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    vLayout->addWidget(button, 0, Qt::AlignHCenter);
    vLayout->addWidget(view, 0, Qt::AlignHCenter);

    QWidget *component = new QWidget(this);
    component->setLayout(vLayout);

    mainLayout->addWidget(component);
}

void MapEditorScreen::onLoadMapClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择地图文件", "maps", "地图文件 (*.txt)");
    if (!filePath.isEmpty()) {
        QFileInfo fileInfo(filePath);
        QString mapName = fileInfo.baseName();
        QString mapsDir = "maps";

        if (fileInfo.absolutePath() != QDir(mapsDir).absolutePath()) {
            QString newFilePath = mapsDir + "/" + mapName + ".txt";
            QFile sourceFile(filePath);
            QFile targetFile(newFilePath);

            if (sourceFile.exists()) {
                if (targetFile.exists()) {
                    QMessageBox::StandardButton reply = QMessageBox::question(this, "确认覆盖", "该地图文件已存在，是否覆盖？",
                                                                             QMessageBox::Yes|QMessageBox::No);
                    if (reply == QMessageBox::No) {
                        return;
                    }
                    if (!targetFile.remove()) {
                        qDebug() << "Failed to remove existing map file:" << newFilePath;
                        return;
                    }
                }

                if (!sourceFile.copy(newFilePath)) {
                    qDebug() << "Failed to copy map file to maps directory:" << newFilePath;
                    return;
                }
                filePath = newFilePath;
            }
        }

        mapEdited->load(filePath);
        drawMap();
        drawGrid();
        mapNameEdit->setText(mapName);
        mapNameEdit->selectAll();
        updateMapList();
    }
}

void MapEditorScreen::onMapListItemClicked(QListWidgetItem *item)
{
    QString mapName = item->text();
    QString filePath = "maps/" + mapName;
    mapEdited->load(filePath);
    drawMap();
    drawGrid();
    mapNameEdit->setText(mapName.left(mapName.length() - 4)); // 去掉 .txt 后缀
    mapNameEdit->selectAll();
}
