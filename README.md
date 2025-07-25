# 2D射击游戏项目说明文档

## 一、模块之间逻辑关系

- **MainWindow（主窗口）**：负责管理各个界面（开始、游戏、暂停、死亡、设置、地图编辑等），通过 QStackedWidget 切换。
- **GameScreen（游戏主界面）**：核心游戏逻辑，包括地图加载、玩家/道具/子弹/护甲生成与管理、碰撞检测、游戏状态更新等。
- **Player（玩家）**：玩家角色，包含移动、攻击、拾取、受伤、地形与装备交互等逻辑。
- **Item/Weapon/Armor/Heal/Defense（道具/武器/护甲/治疗/防御）**：所有可拾取物品的基类与子类，支持多种掉落概率和特殊效果。
- **HUD（状态栏）**：显示玩家血量、武器、护甲及护甲耐久等信息，支持冷却/耐久进度条动态显示。
- **PauseScreen/DeathScreen/StartScreen/SettingsScreen/MapEditorScreen**：各类 UI 界面，负责不同场景下的交互与信息展示。
- **PhysicsEngine**：负责物理碰撞、重力、速度等底层支持。
- **Map/Terrain**：地图与地形管理，支持不同地形对玩家状态的影响（如草地隐身、冰面加速等）。

各模块通过信号槽（Qt Signal/Slot）解耦通信，UI 与逻辑分离，便于维护和扩展。

---

## 二、程序运行流程

1. **启动**：显示 StartScreen，用户可选择开始游戏、设置、地图编辑等。
2. **开始游戏**：MainWindow 切换到 GameScreen，初始化地图、玩家、道具等。
3. **游戏主循环**：
   - 定时器驱动 updateGame()，每帧刷新玩家、道具、子弹、护甲等状态。
   - 玩家可移动、攻击、拾取装备，装备掉落概率由 getSpawnPR 控制。
   - 地形实时影响玩家（如草地隐身、冰面加速）。
   - HUD 实时显示血量、武器、护甲及耐久。
4. **暂停/死亡**：
   - 按 P 键或玩家死亡时，切换到 PauseScreen/DeathScreen。
   - 死亡界面支持重新开始（完整重置游戏状态）或退出。
5. **重新开始/退出**：
   - 重新开始会彻底清理所有对象并重新初始化。
   - 退出返回主菜单。

---

## 三、完成的功能与要求

- [x] 地形与玩家交互（草地隐身、冰面加速等）
- [x] 多种武器、道具、护甲，支持概率掉落
- [x] 护甲系统（锁子甲/防弹衣），支持耐久与特殊防御逻辑
- [x] 死亡界面、暂停界面、主菜单、设置、地图编辑等完整UI
- [x] 游戏状态完整重置（restart）
- [x] HUD美观显示血量、武器、护甲、护甲耐久
- [x] 现代化UI美化（按钮、标题、阴影、渐变等）
- [x] 信号槽解耦，结构清晰，易于维护

（可附运行截图，展示主界面、游戏中、死亡界面、护甲耐久显示等）

---

## 四、参考文献与引用

- Qt 官方文档：https://doc.qt.io/
- 主要用到的 Qt 官方类库如下表：

| 类库/模块           | 主要用途                         |
|---------------------|----------------------------------|
| QtCore              | 基础数据类型、信号槽、定时器等   |
| QtGui               | 绘图、字体、颜色、QImage等       |
| QtWidgets           | 所有窗口控件、布局、QWidget等    |
| QGraphicsView       | 游戏场景、物体管理与渲染         |
| QGraphicsScene      | 游戏场景容器                     |
| QGraphicsItem       | 游戏对象基类                     |
| QStackedWidget      | 多界面切换                       |
| QVBoxLayout/QHBoxLayout/QGridLayout | 界面布局管理 |
| QPushButton/QLabel/QProgressBar/QListWidget | UI控件 |
| QTimer              | 游戏主循环、物品刷新等           |
| QPropertyAnimation  | 冷却/耐久动画效果                |
| QPainter            | 自定义绘图、阴影、渐变等         |
| QFile/QDir          | 文件与资源管理                   |
| QRandomGenerator    | 随机数生成（掉落、AI等）         |
| QFont               | 字体美化                         |
| QPixmap             | 图片处理                         |
| QEvent/QKeyEvent    | 键盘事件处理                     |
| QStyleOptionGraphicsItem | 自定义绘制参数           |

- 参考部分开源项目的 UI 设计思路（如 Github 上的 Qt 游戏项目https://github.com/MeLikeyCode/QtGameEngine?tab=readme-ov-file）
- 代码中如有第三方库/资源，均已在相应文件注明出处

---

## 五、使用的主要库

- Qt 6.5.3（Qt Widgets、Qt Core、Qt GUI、Qt Designer）
- C++ STL（vector、map、algorithm等）

---

如需详细类图、流程图或运行截图，可补充提供。
