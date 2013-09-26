#include <U8glib.h>

//Define V0 pin for LCD light control
#define V0 11
//define LCD data pin
U8GLIB_ST7920_128X64_4X u8g(8, 9, 10, 12, 4, 5, 6, 7, 1, 2, 3);
//Define button pin
#define leftButton A0
#define rightButton A1
#define downButton A2
#define upButton A3
//constant
int button=0;//no: 0; left: 1; right: 2; up: 3; down: t decrease
unsigned long score=0;//total score
int t=500;//one frame time  ( delay time )
//array to control game
unsigned int gameArray[20][10];
unsigned int oldObject[4][2]={{30,30},{30,30},{30,30},{30,30}};
unsigned int newObject[4][2]={{0,0},{0,0},{0,0},{0,0}};
boolean Nextnew=true;
boolean Stop=false;
unsigned int objNum;
boolean Finish=false;
unsigned finishFrame=0;
boolean Skip=false;
unsigned int lowy=0;
unsigned int leftx=10;
unsigned int rightx=0;

//prepare for the font
void u8g_prepare(void) {
  u8g.setFont(u8g_font_4x6);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}

//display score
void scoreDisplay(){
  //change score from int to string
  String strScore=String(score);
  //if score>999999999, then score is too high. Otherwise, display score
  if(strScore.length()<=9){
    u8g.drawStr90(128,1,"Score: ");
    for(int i=0;i<strScore.length();i++){
      switch(strScore[i]){
        case '0': u8g.drawStr90(128,7*4+4*i,"0");break;
        case '1': u8g.drawStr90(128,7*4+4*i,"1");break;
        case '2': u8g.drawStr90(128,7*4+4*i,"2");break;
        case '3': u8g.drawStr90(128,7*4+4*i,"3");break;
        case '4': u8g.drawStr90(128,7*4+4*i,"4");break;
        case '5': u8g.drawStr90(128,7*4+4*i,"5");break;
        case '6': u8g.drawStr90(128,7*4+4*i,"6");break;
        case '7': u8g.drawStr90(128,7*4+4*i,"7");break;
        case '8': u8g.drawStr90(128,7*4+4*i,"8");break;
        case '9': u8g.drawStr90(128,7*4+4*i,"9");break;
        default : break;
      }
    }  
  }else{
    u8g.drawStr90(128,0,"Super Score !!!");
  }
}
///////////////////////////////////////////////////////////////////
//display game
void gameDisplay(){
  //Game Frame
  if(!Finish){
    u8g.drawFrame(0,1,121,62);
    for(int i=0;i<20;i++){
      for(int j=0;j<10;j++){
        if(gameArray[i][j]==1){
          u8g.drawBox(120-6*(i+1),2+j*6,6,6);
        }
      }
    }
  }else{
    u8g.drawStr90(64,0,"You are dead !!");
  }
}

void draw(){
  //prepare font
  u8g_prepare();
  //display score
  scoreDisplay();
  //display game
  gameDisplay();
}

///////////////////////////////////////////////////////////////////////////////////////////
void Calarray(){
  //if Nextnew is true, generate a new object
  if(Nextnew){
    objNum=random(4);
    switch(objNum){
      case 0:
      //1111
        for(int i=0;i<4;i++){
          newObject[i][0]=0;
          newObject[i][1]=3+i;
        }
        break;
      case 1:
      //11
      //11
        for(int i=0;i<4;i++){
          if(i<2){
            newObject[i][0]=0;
          }else{newObject[i][0]=1;}
          if(i%2==0){
            newObject[i][1]=4;
          }else{newObject[i][1]=5;}
        }
        break;
      case 2:
      //111
      //1
        for(int i=0;i<3;i++){
          newObject[i][0]=0;
          newObject[i][1]=4+i;
        }
        newObject[3][0]=1;
        newObject[3][1]=4;
        break;
      case 3:
      //111
      // 1
        for(int i=0;i<3;i++){
          newObject[i][0]=0;
          newObject[i][1]=4+i;
        }
        newObject[3][0]=1;
        newObject[3][1]=5;
        break;
      default:break;
    }
    //stop generate
    Nextnew=false;
    //if newObject has been ocupy, dead!!
    for(int i=0;i<4;i++){
      if(gameArray[newObject[i][0]][newObject[i][1]]==1){
        Finish=true;break;
      }
    }
  }else{
    //normal situation
    switch(button){
      case 1:
      //left
      leftx=10;
      //copy newObject to oldObject
      for(int i=0;i<4;i++){
        for(int j=0;j<2;j++){
          oldObject[i][j]=newObject[i][j];
        }
      }
      break;
      case 2:
      //right
      score=20;
      break;
      case 3:
      //up
      score=30;
      break;
      default:
      //down
      lowy=0;
      //copy newObject to oldObject
      for(int i=0;i<4;i++){
        for(int j=0;j<2;j++){
          oldObject[i][j]=newObject[i][j];
        }
      }
      //newObject go down
      for(int i=0;i<4;i++){
        newObject[i][0]+=1;
        //if get down, Stop=true
        if(newObject[i][0]>19){Stop=true;break;}
        if(newObject[i][0]>lowy){lowy=newObject[i][0];}
      }
      //some obejct may stop moving object
      for(int i=0;i<4;i++){
        if(newObject[i][0]==lowy){
          if(gameArray[lowy][newObject[i][1]]==1){
            Stop=true;
            break;
          }
        }
      }
      break;
    }
  }
  if(Stop || Finish){
    Stop=false;
    Nextnew=true;
    for(int i=0;i<4;i++){
      for(int j=0;j<2;j++){
        oldObject[i][j]=30;
      }
    }
  }else{
    if(Skip){
      Skip=false;
      for(int i=0;i<4;i++){
        for(int j=0;j<2;j++){
          newObject[i][j]=oldObject[i][j];
        }
      }
    }
    //clear oldObject
    for(int i=0;i<4;i++){
        if(oldObject[i][0]<20 && oldObject[i][1]<10){
          gameArray[oldObject[i][0]][oldObject[i][1]]=0;
      }
    }
    //generate newObject
    for(int i=0;i<4;i++){
      gameArray[newObject[i][0]][newObject[i][1]]=1;
    }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup(){
  //initial random seed
  randomSeed(analogRead(A5));
  //initial game array
  for(int i=0;i<20;i++){
    for(int j=0;j<10;j++){
      gameArray[i][j]=0;
    }
  }
  //LCD light
  analogWrite(V0,255/3*2);
  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) 
    u8g.setColorIndex(255);     // white
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT )
    u8g.setColorIndex(3);         // max intensity
  else if ( u8g.getMode() == U8G_MODE_BW )
    u8g.setColorIndex(1);         // pixel on
}

void loop(){
  //judge button
  if(analogRead(leftButton)>500){
    delay(100);
    if(analogRead(leftButton)>500){
      button=1;
    }
  }else if(analogRead(rightButton)>500){
    delay(100);
    if(analogRead(rightButton)>500){
      button=2;
    }
  }else if(analogRead(downButton)){
    delay(100);
    if(analogRead(downButton)){
      t=5;
    }
  }else if(analogRead(upButton)){
    delay(100);
    if(analogRead(upButton)){
      button=3;
    }
  }else{button=0;t=500;}
  if(!Finish){
    /////////////////////////////////////////////////////////////////////////////
    //calculate array
    Calarray();
  }
  //Draw on the LCD
  u8g.firstPage();//clear screen
  //draw new figure
  do{draw();}while(u8g.nextPage());
  //delay t ms
  delay(t);
}
