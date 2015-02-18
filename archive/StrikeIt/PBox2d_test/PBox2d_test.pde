import pbox2d.*;
import org.jbox2d.dynamics.*;
import org.jbox2d.common.*;
import org.jbox2d.collision.shapes.*;

PBox2D box2d;
ArrayList<Body> coins;
 
void setup() {  
  box2d = new PBox2D(this);
  box2d.createWorld();//Initializes a Box2D world with default settings
  
  //remove gravity
  box2d.setGravity(0,0);

  Body coin1 = createCoin(100, 100, 5);
  coin1.applyForce(new Vec2(2,2), coin1.getWorldCenter());
}

void draw()
{
  box2d.step();
  
  strokeWeight(3);
  for(Body coin: coins)
  {
   Vec2 posWorld = box2d.getBodyPixelCoord(coin);
   Vec2 posPixel = box2d.coordWorldToPixels(posWorld);
   
   point(posPixel.x, posPixel.y);
  }
}

Body createCoin(float xPos, float yPos, float radius)
{
  //tuning variables
  float linearDampingOfPieces = 0.8;
  boolean arePiecesBullets = true;
  float coinDensity = 1;

  //define the behavior for strikers and coins (called pieces)
  BodyDef pieceDef = new BodyDef();
  pieceDef.linearDamping = linearDampingOfPieces;
  pieceDef.bullet = arePiecesBullets;
  
  Body coin = box2d.createBody(pieceDef);
  
  CircleShape coinShape = new CircleShape();
  coinShape.m_radius = box2d.scalarPixelsToWorld(radius);
  coinShape.m_p = box2d.coordPixelsToWorld(xPos,yPos);
  
  FixtureDef coinFD = new FixtureDef();
  coinFD.shape = coinShape;
  coinFD.density = coinDensity;
  
  coin.createFixture(coinFD);
  
  return coin;
}
