#include "game_object.h"
#include "player.h"
#include "config.h"
#include "../physics/physics_engine.h"
#include "../map/map.h"

GameObject::GameObject(bool movable) :
    movable(movable), grounded(false)
{
    phEn = PhysicsEngine::getInstance();
}

GameObject::~GameObject() {}

QPointF GameObject::vel() const {
    return m_vel;
} 

void GameObject::setVel(const QPointF newvel) {
    m_vel = newvel;
}

void GameObject::update() {
    if (!movable) return;

    if (grounded) {
        m_vel.setY(0);
        phEn->applyFriction(this);
    } else {
        phEn->applyGravity(this);
        if (dynamic_cast<Player*>(this)) phEn->applyFriction(this);
    }

    // 限制终端速度（如果继承了MotionProfile）
    MotionProfile* mp = dynamic_cast<MotionProfile*>(this);
    if (mp) {
        float maxVx = mp->getTerminalVelocityX();
        float maxVy = mp->getTerminalVelocityY();
        if (std::abs(m_vel.x()) > maxVx) m_vel.setX((m_vel.x() > 0 ? 1 : -1) * maxVx);
        if (std::abs(m_vel.y()) > maxVy) m_vel.setY((m_vel.y() > 0 ? 1 : -1) * maxVy);
    }

    // update position
    setPos(pos() + m_vel);

    // boundaries
    checkBoundaries();    
    
    processCollisionWithTerrains(Map::getInstance());

    // boundaries
    checkBoundaries();
}

QRectF GameObject::boundingRect() const {
    return collideBox;
}

bool GameObject::isMovable() const {
    return movable;
}

bool GameObject::onGround() const {
    return grounded;
}

void GameObject::setOnGround(bool value){
    grounded = value;
}

bool GameObject::checkNotOnGround() {
    bool boo1 = false;
    bool boo2 = false;

    QRect gridRect = getGrid();
    int minX = gridRect.left();
    int minY = gridRect.top();
    int maxX = gridRect.right();
    int maxY = gridRect.bottom();

    for (int y = maxY; y >= minY; --y) {
        for (int x = minX; x <= maxX; ++x) {
            Terrain* terrain = Map::getInstance()->map[y][x];
            if (terrain && terrain->getTypeId() != 0) {
                QRectF overlap = terrain->getPosRectF().intersected(getPosRectF());
                if (!overlap.isEmpty()) {
                    boo1 = true;
                    break;
                }
            }
        }
        if (boo1) break;
    }

    for (int x = minX; x <= maxX; ++x) {
        Terrain* terrainBelow = Map::getInstance()->map[maxY][x];
        if (terrainBelow && terrainBelow->getTypeId() != 0) {
            boo2 = true;
            break;
        }
    }

    return boo1 || !boo2;  // falling and collided with terrains
}

bool GameObject::isOutside() const {
    return pos().x() < 0 || pos().x() + boundingRect().width() > GAME_WIDTH ||
           pos().y() + boundingRect().height() > GAME_HEIGHT;
}

void GameObject::checkBoundaries() {
    if (!movable) return;
    if (pos().x() < 0) {
        setPos(QPoint(0, pos().y()));
        setVel(QPoint(-vel().x(), vel().y()));
    } else if (pos().x() + boundingRect().width() > GAME_WIDTH) {
        setPos(QPoint(GAME_WIDTH - boundingRect().width(), pos().y()));
        setVel(QPoint(-vel().x(), vel().y()));
    }
    if (pos().y() + boundingRect().height() > GAME_HEIGHT) {
        setPos(QPoint(pos().x(), GAME_HEIGHT - boundingRect().height()));
        setVel(QPoint(vel().x(), 0));
    }
}

QPoint GameObject::positionToGrid(const QPointF &pos) const {
    int x = static_cast<int>(pos.x()) / TERRAIN_WIDTH;
    int y = static_cast<int>(pos.y()) / TERRAIN_HEIGHT;
    return QPoint(x, y);
}

QRectF GameObject::getPosRectF() const {
    return QRectF(pos(), boundingRect().size());
}

QRectF GameObject::getGridRectF() const {
    // get the rectangle that covers the grid cells this object occupies

    QRect gridRect = getGrid();
    int minX = gridRect.left() * TERRAIN_WIDTH;
    int minY = gridRect.top() * TERRAIN_HEIGHT;
    int maxX = (gridRect.right() + 1) * TERRAIN_WIDTH;
    int maxY = (gridRect.bottom() + 1) * TERRAIN_HEIGHT;

    return QRectF(QPointF(minX, minY), QSizeF(maxX - minX, maxY - minY));
}

QRect GameObject::getGrid() const {
    int minX = std::max(0, static_cast<int>(pos().x() / TERRAIN_WIDTH));
    int minY = std::max(0, static_cast<int>(pos().y() / TERRAIN_HEIGHT));
    int maxX = std::min(GAME_WIDTH / TERRAIN_WIDTH,
                        static_cast<int>(std::ceil((pos().x() + boundingRect().width()) / TERRAIN_WIDTH)));
    int maxY = std::min(GAME_HEIGHT / TERRAIN_HEIGHT,
                        static_cast<int>(std::ceil((pos().y() + boundingRect().height()) / TERRAIN_HEIGHT)));

    return QRect(QPoint(minX, minY), QSize(maxX - minX, maxY - minY));
}

QRect GameObject::getGrid(const QPointF pf) const {
    int minX = std::max(0, static_cast<int>(pf.x() / TERRAIN_WIDTH));
    int minY = std::max(0, static_cast<int>(pf.y() / TERRAIN_HEIGHT));
    int maxX = std::min(GAME_WIDTH / TERRAIN_WIDTH - 1,
                        static_cast<int>(std::ceil((pf.x() + boundingRect().width()) / TERRAIN_WIDTH)) - 1);
    int maxY = std::min(GAME_HEIGHT / TERRAIN_HEIGHT - 1,
                        static_cast<int>(std::ceil((pf.y() + boundingRect().height()) / TERRAIN_HEIGHT)) - 1);

    return QRect(QPoint(minX, minY), QSize(maxX - minX, maxY - minY));
}

void GameObject::processCollisionWithTerrains(Map* gameMap) {
    // 获取所在grid大小和位置编号
    QRect gridRect = getGrid();
    int minX = gridRect.left();
    int minY = gridRect.top();
    int maxX = gridRect.right();
    int maxY = gridRect.bottom();

    QList<CollisionInfo> collisionInfo;

    if (grounded) {
        bool standingOnTerrain = false;
        for (int x = minX; x <= maxX; ++x) {
            Terrain* terrain = gameMap->map[maxY][x];
            if (terrain && terrain->getTypeId() != 0) {
                // qDebug() << "Processing terrain at (" << x << ", " << maxY << ")";
                QRectF overlap = terrain->getPosRectF().intersected(getPosRectF());
                if (!overlap.isEmpty()) {
                    // 位置处理
                    collisionInfo.append(phEn->getCollisionInfo(this, terrain));
                    // 逻辑处理
                    // if (collisionInfo.last().direction == Direction::TOP) terrain->onCollidedWith(this);
                }
            }

            Terrain* terrainBelow = gameMap->map[maxY+1][x];
            if (terrainBelow && terrainBelow->getTypeId() != 0) {
                standingOnTerrain = true;
                setVel(QPointF(vel().x(), 0)); // 停止下落
                break;
            }
        }
        setOnGround(standingOnTerrain);
    } else {
        for (int y = maxY; y >= minY; --y) {
            for (int x = minX; x <= maxX; ++x) {
                Terrain* terrain = gameMap->map[y][x];
                if (terrain && terrain->getTypeId() != 0) {
                    // qDebug() << "Processing terrain at (" << x << ", " << y << ")";
                    QRectF overlap = terrain->getPosRectF().intersected(getPosRectF());
                    if (!overlap.isEmpty()) {
                        // qDebug() << "Collision detected with terrain at (" << x << ", " << y << ")";
                        // 位置处理
                        collisionInfo.append(phEn->getCollisionInfo(this, terrain));
                        // qDebug() << "Collision Direction:" << collisionInfo.last().direction;
                    }
                }
            }
        }
    }

    // 处理碰撞信息
    // 从水平和垂直中各选出一个
    // sort
    std::sort(collisionInfo.begin(), collisionInfo.end(), [](const CollisionInfo &a, const CollisionInfo &b) {
        bool isVerticalA = (a.direction == Direction::TOP || a.direction == Direction::BOTTOM);
        bool isVerticalB = (b.direction == Direction::TOP || b.direction == Direction::BOTTOM);
        if (isVerticalA && isVerticalB) {
            return a.overlap.width() > b.overlap.width();
        } else if (!isVerticalA && !isVerticalB) {
            return a.overlap.height() > b.overlap.height();
        } else {
            return isVerticalA; // 优先处理垂直方向的碰撞
        }
    });

    if (!collisionInfo.isEmpty()) {
        // 处理第一个碰撞信息
        phEn->handleCollision(collisionInfo.first());
        // 递归处理
        // processCollisionWithTerrains(gameMap);
    } else {
        // qDebug() << "No collisions detected.";
    }
}
