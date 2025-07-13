#ifndef MAP_EDITOR_SCREEN_H
#define MAP_EDITOR_SCREEN_H

#include "screen.h"
#include "../map/map.h"
#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QButtonGroup>
#include <QLabel>

class MapEditorScreen : public Screen
{
    Q_OBJECT

public:
    explicit MapEditorScreen(QWidget *parent = nullptr);
    ~MapEditorScreen();

signals:
    void backToStartScreen();

private slots:
    void saveMap();
    void onCellClicked(const QPointF& pos);
    void onTerrainSelected(int id);
    void onSelectMapClicked();
    void onMapListItemClicked(QListWidgetItem *item); // 新增槽函数

private:
    Map *mapEdited;
    QListWidget *mapListWidget;
    QPushButton *saveButton;
    QLineEdit *mapNameEdit;
    QGraphicsScene *editorScene;
    QGraphicsView *editorView;
    int selectedTerrainType;
    QButtonGroup *terrainButtonGroup;
    QPushButton *selectMapButton;
    QPushButton *exitButton;

    void updateMapList();
    void drawMap();
    void drawGrid();
    bool eventFilter(QObject *obj, QEvent *event) override;

    void setupTerrainSelection();
    void createTerrainButtonAndView(const QString& buttonText, int id, const QString& path,
                                    QButtonGroup* buttonGroup, QHBoxLayout* mainLayout);
};

#endif // MAP_EDITOR_SCREEN_H
