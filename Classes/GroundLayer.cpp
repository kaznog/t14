//
//  GroundLayer.cpp
//  b2dcar
//
//  Created by 野口一也 on 2014/07/11.
//
//

#include "GroundLayer.h"

USING_NS_CC;

GroundLayer::GroundLayer()
: _world(nullptr)
, _car(nullptr)
, _controlState(0x0)
{
    
}

GroundLayer::~GroundLayer()
{
    //call DestroyBody for every body to invoke destruction listener to clear fixture user data automatically
    for ( b2Body* b = _world->GetBodyList(); b ; ) {
        b2Body* nextBody = b->GetNext();
        _world->DestroyBody( b );
        b = nextBody;
    }
    CC_SAFE_DELETE(_world);
    CC_SAFE_DELETE(_car);
}

// on "init" you need to initialize your instance
bool GroundLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto director = Director::getInstance();
    _winSize = director->getWinSize();
    _scale = _winSize.height/100;
    this->setScale(_scale);
    log("scale: %f", _scale);

//    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0, 0));
    this->setPosition(_winSize.width/2, _winSize.height/2);
    
    initPhysics();
    scheduleUpdate();
    return true;
}

void GroundLayer::initPhysics()
{
    loadWorld();
    _car = new Car(_world, Vec2(0, 0));
}

void GroundLayer::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    if ( !_world )
        return;
    
    // debug draw display will be on top of anything else
    Layer::draw(renderer, transform, flags);
    
    _customCmd.init(_globalZOrder);
    _customCmd.func = CC_CALLBACK_0(GroundLayer::onDraw, this, transform, flags);
    renderer->addCommand(&_customCmd);
}

void GroundLayer::onDraw(const Mat4 &transform, uint32_t flags)
{
    Director* director = Director::getInstance();
    CCASSERT(nullptr != director, "Director is null when seting matrix stack");
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
    
    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );


    for (b2Body* b = _world->GetBodyList(); b; b = b->GetNext()) {
        const b2Transform& xf = b->GetTransform();
        for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
        {
            if (b->IsActive() == false)
            {
                DrawShape(f, xf, b2Color(0.5f, 0.5f, 0.3f));
            }
            else if (b->GetType() == b2_staticBody)
            {
                DrawShape(f, xf, b2Color(0.5f, 0.9f, 0.5f));
            }
            else if (b->GetType() == b2_kinematicBody)
            {
                DrawShape(f, xf, b2Color(0.5f, 0.5f, 0.9f));
            }
            else if (b->IsAwake() == false)
            {
                DrawShape(f, xf, b2Color(0.6f, 0.6f, 0.6f));
            }
            else
            {
                DrawShape(f, xf, b2Color(0.9f, 0.7f, 0.7f));
            }
        }
    }
    
//    _world->DrawDebugData();
    CHECK_GL_ERROR_DEBUG();
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void GroundLayer::DrawShape(b2Fixture *fixture, const b2Transform &xf, const b2Color &color)
{
    b2Vec2 carPos = _car->m_body->GetPosition();
    Vec3 transPos = Vec3(carPos.x, carPos.y, 0);
    float angle = -1 * _car->m_body->GetAngle();
	switch (fixture->GetType())
	{
        case b2Shape::e_circle:
		{
			b2CircleShape* circle = (b2CircleShape*)fixture->GetShape();
            
			b2Vec2 center = b2Mul(xf, circle->m_p);
			float32 radius = circle->m_radius;
			b2Vec2 axis = b2Mul(xf.q, b2Vec2(1.0f, 0.0f));
            
			_debugDraw->DrawSolidCircle(center, radius, axis, color, transPos, angle);
		}
            break;
            
        case b2Shape::e_edge:
		{
			b2EdgeShape* edge = (b2EdgeShape*)fixture->GetShape();
			b2Vec2 v1 = b2Mul(xf, edge->m_vertex1);
			b2Vec2 v2 = b2Mul(xf, edge->m_vertex2);
			_debugDraw->DrawSegment(v1, v2, color, transPos, angle);
		}
            break;
            
        case b2Shape::e_chain:
		{
			b2ChainShape* chain = (b2ChainShape*)fixture->GetShape();
			int32 count = chain->m_count;
			const b2Vec2* vertices = chain->m_vertices;
            
			b2Vec2 v1 = b2Mul(xf, vertices[0]);
			for (int32 i = 1; i < count; ++i)
			{
				b2Vec2 v2 = b2Mul(xf, vertices[i]);
				_debugDraw->DrawSegment(v1, v2, color, transPos, angle);
				_debugDraw->DrawCircle(v1, 0.05f, color, transPos, angle);
				v1 = v2;
			}
		}
            break;
            
        case b2Shape::e_polygon:
		{
			b2PolygonShape* poly = (b2PolygonShape*)fixture->GetShape();
			int32 vertexCount = poly->m_count;
			b2Assert(vertexCount <= b2_maxPolygonVertices);
			b2Vec2 vertices[b2_maxPolygonVertices];
            
			for (int32 i = 0; i < vertexCount; ++i)
			{
				vertices[i] = b2Mul(xf, poly->m_vertices[i]);
			}
            
			_debugDraw->DrawSolidPolygon(vertices, vertexCount, color, transPos, angle);
		}
            break;
            
        default:
            break;
	}
}

void GroundLayer::update(float dt)
{
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 10;
    int32 positionIterations = 10;
    _world->Step(timeStep, velocityIterations, positionIterations);
    _world->ClearForces();
    setViewPointCenter();
    
    if (_car != nullptr) {
        _car->update(_controlState);
    }
}

void GroundLayer::setViewPointCenter()
{
    b2Vec2 carPos = _car->m_body->GetPosition();
    Vec2 CenterPos = Vec2((carPos.x * _scale - (_winSize.width / 2)), (carPos.y * _scale - (_winSize.height / 2)));
    this->setPosition(-1 * CenterPos);
}

void GroundLayer::TouchBegan(int CtrlState)
{
    switch (CtrlState) {
        case CTRL_LEFT:
            _controlState &= ~CTRL_RIGHT;
            _controlState |= CTRL_LEFT;
            break;
            
        case CTRL_RIGHT:
            _controlState &= ~CTRL_LEFT;
            _controlState |= CTRL_RIGHT;
            break;
            
        case CTRL_ACCEL:
            _controlState &= ~CTRL_BACK;
            _controlState |= CTRL_ACCEL;
            break;
            
        case CTRL_BACK:
            _controlState &= ~CTRL_ACCEL;
            _controlState |= CTRL_BACK;
            break;
            
        default:
            break;
    }
}

void GroundLayer::TouchEnded(int CtrlState)
{
    switch (CtrlState) {
        case CTRL_LEFT:
            _controlState &= ~CTRL_LEFT;
            break;
            
        case CTRL_RIGHT:
            _controlState &= ~CTRL_RIGHT;
            break;
            
        case CTRL_ACCEL:
            _controlState &= ~CTRL_ACCEL;
            break;

        case CTRL_BACK:
            _controlState &= ~CTRL_BACK;
            break;
            
        default:
            break;
    }
}

void GroundLayer::loadWorld()
{
    // Get the name of the .json file to load, eg. "jointTypes.json"
    std::string filename = "racetrack.json";
    
    // Find out the absolute path for the file
    std::string fullpath = FileUtils::getInstance()->fullPathForFilename(filename.c_str());
    
    // This will print out the actual location on disk that the file is read from.
    // When using the simulator, exporting your RUBE scene to this folder means
    // you can edit the scene and reload it without needing to restart the app.
    CCLOG("Full path is: %s", fullpath.c_str());
    
    // Create the world from the contents of the RUBE .json file. If something
    // goes wrong, m_world will remain NULL and errMsg will contain some info
    // about what happened.
    b2dJson json;
    std::string errMsg;
    ssize_t fileSize = 0;
    unsigned char* fileData = FileUtils::getInstance()->getFileData(fullpath.c_str(), "r", &fileSize);
    std::string jsonContent;
    jsonContent.assign(reinterpret_cast<const char*>(fileData), fileSize);
    _world = json.readFromString(jsonContent, errMsg);
    
    if ( _world ) {
        CCLOG("Loaded JSON ok");
        
        _debugDraw = new GLESDebugDraw(PTM_RATIO);
        uint32 flags = 0;
        flags += b2Draw::e_shapeBit;
//        flags += b2Draw::e_jointBit;
//        flags += b2Draw::e_pairBit;
//        flags += b2Draw::e_aabbBit;
//        flags += b2Draw::e_centerOfMassBit;
        _debugDraw->SetFlags(flags);
        _world->SetDebugDraw(_debugDraw);
        
        afterLoadProcessing(&json);
    } else {
        log("%s",errMsg.c_str());
    }
}

void GroundLayer::afterLoadProcessing(b2dJson *json)
{
    b2BodyDef bodyDef;
    b2Body* m_groundBody = _world->CreateBody(&bodyDef);

    b2FrictionJointDef frictionJointDef;
    frictionJointDef.localAnchorA.SetZero();
    frictionJointDef.localAnchorB.SetZero();
    frictionJointDef.bodyA = m_groundBody;
    frictionJointDef.maxForce = 400;
    frictionJointDef.maxTorque = 400;
    frictionJointDef.collideConnected = true;
    
    std::vector<b2Body*> barrelBodies;
    json->getBodiesByName("barrel", barrelBodies);
    for (int i = 0; i < barrelBodies.size(); i++) {
        b2Body* barrelBody = barrelBodies[i];
        frictionJointDef.bodyB = barrelBody;
        _world->CreateJoint( &frictionJointDef );
    }
    
//    std::vector<b2Fixture*> waterFixtures;
//    json->getFixturesByName("water", waterFixtures);
//    for (int i = 0; i < waterFixtures.size(); i++) {
//        b2Fixture* waterFixture = waterFixtures[i];
//    }
    // 重力を作成
    b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
    _world->SetGravity(gravity);
    _world->SetAllowSleeping(true);
    _world->SetContinuousPhysics(true);

}