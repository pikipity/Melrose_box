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
int t=33;//one frame time  ( delay time )
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
unsigned int displayState=0;
unsigned int displayLine[20];
boolean needRemove=false;
unsigned int FrameNum=0;

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

void CreatNewObject(){
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
}

void ButtonResponse(){
  //button response
    switch(button){
      case 1:
      //left
      //copy newObject to oldObject
      for(int i=0;i<4;i++){
        for(int j=0;j<2;j++){
          oldObject[i][j]=newObject[i][j];
        }
      }
      //newObkject go left
      for(int i=0;i<4;i++){
        if(newObject[i][1]==0){Skip=true;break;}else{newObject[i][1]-=1;}
      }
      //some obejct may stop moving left
      for(int i=0;i<4;i++){
          if(gameArray[newObject[i][0]][newObject[i][1]]==1){
            boolean IsStopByself;
            for(int j=0;j<4;j++){
              if(newObject[i][0]==oldObject[j][0] && newObject[i][1]==oldObject[j][1]){
                IsStopByself=true;
                break;
              }else{
                IsStopByself=false;
              }
            }
            if(!IsStopByself){
              Skip=true;
              break;
            }
          }
      }
      break;
      case 2:
      //right
      //copy newObject to oldObject
      for(int i=0;i<4;i++){
        for(int j=0;j<2;j++){
          oldObject[i][j]=newObject[i][j];
        }
      }
      //newObkject go right
      for(int i=0;i<4;i++){
        if(newObject[i][1]==9){Skip=true;break;}else{newObject[i][1]+=1;}
      }
      //some obejct may stop moving left
      for(int i=0;i<4;i++){
          if(gameArray[newObject[i][0]][newObject[i][1]]==1){
            boolean IsStopByself;
            for(int j=0;j<4;j++){
              if(newObject[i][0]==oldObject[j][0] && newObject[i][1]==oldObject[j][1]){
                IsStopByself=true;
                break;
              }else{
                IsStopByself=false;
              }
            }
            if(!IsStopByself){
              Skip=true;
              break;
            }
          }
      }
      break;
      case 3:
      //up
      //copy newObject to oldObject
      for(int i=0;i<4;i++){
        for(int j=0;j<2;j++){
          oldObject[i][j]=newObject[i][j];
        }
      }
      //change
      switch(objNum){
        case 0:
        //1111
            if(newObject[0][0]==newObject[1][0] && newObject[0][1]==newObject[1][1]-1){
              if(newObject[0][0]>=2 && newObject[3][0]<=18){
                newObject[0][0]-=2;
                newObject[0][1]+=2;
                newObject[1][0]-=1;
                newObject[1][1]+=1;
                newObject[3][0]+=1;
                newObject[3][1]-=1;
              }else{
                Skip=true;
                break;
              }
            }else if(newObject[0][0]==newObject[1][0] && newObject[0][1]==newObject[1][1]+1){
              if(newObject[0][0]<=17 && newObject[3][0]>=1){
                newObject[0][0]+=2;
                newObject[0][1]-=2;
                newObject[1][0]+=1;
                newObject[1][1]-=1;
                newObject[3][0]-=1;
                newObject[3][1]+=1;
              }else{
                Skip=true;
                break;
              }
            }else if(newObject[0][0]==newObject[1][0]-1 && newObject[0][1]==newObject[1][1]){
              if(newObject[0][1]<=7 && newObject[3][1]>=1){
                newObject[0][0]+=2;
                newObject[0][1]+=2;
                newObject[1][0]+=1;
                newObject[1][1]+=1;
                newObject[3][0]-=1;
                newObject[3][1]-=1;
              }else{
                Skip=true;
                break;
              }
            }else if(newObject[0][0]==newObject[1][0]+1 && newObject[0][1]==newObject[1][1]){
              if(newObject[0][1]>=2 && newObject[3][1]<=8){
                newObject[0][0]-=2;
                newObject[0][1]-=2;
                newObject[1][0]-=1;
                newObject[1][1]-=1;
                newObject[3][0]+=1;
                newObject[3][1]+=1;
              }else{
                Skip=true;
                break;
              }
            }
        break;
        case 2:
        //111
        //1
            if(newObject[0][0]==newObject[1][0] && newObject[0][1]==newObject[1][1]-1){
              if(newObject[0][0]>=1 && newObject[2][0]<=19){
                newObject[0][0]-=1;
                newObject[0][1]+=1;
                newObject[3][0]-=2;
                newObject[2][0]+=1;
                newObject[2][1]-=1;
              }else{
                Skip=true;
                break;
              }
            }else if(newObject[0][0]==newObject[1][0] && newObject[0][1]==newObject[1][1]+1){
              if(newObject[0][0]<=18 && newObject[2][0]>=1){
                newObject[0][0]+=1;
                newObject[0][1]-=1;
                newObject[3][0]+=2;
                newObject[2][0]-=1;
                newObject[2][1]+=1;
              }else{
                Skip=true;
                break;
              }
            }else if(newObject[0][0]==newObject[1][0]-1 && newObject[0][1]==newObject[1][1]){
              if(newObject[0][1]<=8 && newObject[2][1]>=1){
                newObject[0][0]+=1;
                newObject[0][1]+=1;
                newObject[3][1]+=2;
                newObject[2][0]-=1;
                newObject[2][1]-=1;
              }else{
                Skip=true;
                break;
              }
            }else if(newObject[0][0]==newObject[1][0]+1 && newObject[0][1]==newObject[1][1]){
              if(newObject[0][1]>=1 && newObject[2][1]<=8){
                newObject[0][0]-=1;
                newObject[0][1]-=1;
                newObject[3][1]-=2;
                newObject[2][0]+=1;
                newObject[2][1]+=1;
              }else{
                Skip=true;
                break;
              }
            }        
        break;
        case 3:
        //111
        // 1
            if(newObject[1][0]==newObject[3][0]-1 && newObject[1][1]==newObject[3][1]){
              if(newObject[0][0]>=1){
                newObject[0][0]-=1;
                newObject[0][1]+=1;
                newObject[3][0]-=1;
                newObject[3][1]-=1;
                newObject[2][0]+=1;
                newObject[2][1]-=1;
              }else{
                Skip=true;
                break;
              }
            }else if(newObject[1][0]==newObject[3][0] && newObject[1][1]==newObject[3][1]+1){
              if(newObject[0][1]<=8){
                newObject[0][0]+=1;
                newObject[0][1]+=1;
                newObject[3][0]-=1;
                newObject[3][1]+=1;
                newObject[2][0]-=1;
                newObject[2][1]-=1;
              }else{
                Skip=true;
                break;
              }
            }else if(newObject[1][0]==newObject[3][0]+1 && newObject[1][1]==newObject[3][1]){
              if(newObject[0][0]<=18){
                newObject[0][0]+=1;
                newObject[0][1]-=1;
                newObject[3][1]+=1;
                newObject[3][0]+=1;
                newObject[2][0]-=1;
                newObject[2][1]+=1;
              }else{
                Skip=true;
                break;
              }
            }else if(newObject[1][0]==newObject[3][0] && newObject[1][1]==newObject[3][1]-1){
              if(newObject[0][1]>=1){
                newObject[0][0]-=1;
                newObject[0][1]-=1;
                newObject[3][1]-=1;
                newObject[3][0]+=1;
                newObject[2][0]+=1;
                newObject[2][1]+=1;
              }else{
                Skip=true;
                break;
              }
            }       
        break;
        default:break;
      }
      //some obejct may stop moving
      for(int i=0;i<4;i++){
          if(gameArray[newObject[i][0]][newObject[i][1]]==1){
            boolean IsStopByself;
            for(int j=0;j<4;j++){
              if(newObject[i][0]==oldObject[j][0] && newObject[i][1]==oldObject[j][1]){
                IsStopByself=true;
                break;
              }else{
                IsStopByself=false;
              }
            }
            if(!IsStopByself){
              Skip=true;
              break;
            }
          }
      }
      break;
      default:
      //down
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
      }
      //some obejct may stop moving object
      for(int i=0;i<4;i++){
          if(gameArray[newObject[i][0]][newObject[i][1]]==1){
            boolean IsStopByself;
            for(int j=0;j<4;j++){
              if(newObject[i][0]==oldObject[j][0] && newObject[i][1]==oldObject[j][1]){
                IsStopByself=true;
                break;
              }else{
                IsStopByself=false;
              }
            }
            if(!IsStopByself){
              Stop=true;
              break;
            }
          }
      }
      break;
    }
}

void IfneedRemove(){
  if(!needRemove){
      int n=0;
      for(int i=19;i>=0;i--){
        boolean addLine;
        for(int j=0;j<10;j++){
          if(gameArray[i][j]!=1){
            addLine=false;
            break;
          }else{
            addLine=true;
          }
        }
        if(addLine){
          needRemove=true;
          displayLine[n]=i;
          n++;
          addLine=false;
        }
      }
    }
}

void RemoveLine(){
  //copy newObject to oldObject
      for(int i=0;i<4;i++){
        for(int j=0;j<2;j++){
          oldObject[i][j]=newObject[i][j];
        }
      }
    //
    if(displayState==0){
        score+=10;
        for(int i=0;i<20;i++){
          if(displayLine[i]<20){
            for(int j=0;j<10;j++){
              gameArray[displayLine[i]][j]=0;
            }
          }
        }
        displayState++;
    }else if(displayState==1){
        for(int i=0;i<20;i++){
          if(displayLine[i]<20){
            for(int j=0;j<10;j++){
              gameArray[displayLine[i]][j]=1;
            }
          }
        }
        displayState++;
    }else if(displayState==2){
        for(int i=0;i<20;i++){
          if(displayLine[i]<20){
            for(int j=0;j<10;j++){
              gameArray[displayLine[i]][j]=0;
            }
          }
        }
        displayState++;
    }else if(displayState==3){
        for(int i=0;i<20;i++){
          if(displayLine[i]<20){
            for(int j=0;j<10;j++){
              gameArray[displayLine[i]][j]=1;
            }
          }
        }
        displayState++;
    }else if(displayState==4){
        for(int i=0;i<20;i++){
          if(displayLine[i]<20){
            for(int j=0;j<10;j++){
              gameArray[displayLine[i]][j]=0;
            }
          }
        }
        displayState++;
    }else if(displayState==5){
        for(int i=0;i<4;i++){
          gameArray[newObject[i][0]][newObject[i][1]]=0;
        }
        for(int i=0;i<20;i++){
          if(displayLine[i]<20){
            for(int n=displayLine[i];n>=0;n--){
              for(int j=0;j<10;j++){
                gameArray[n][j]=gameArray[n-1][j];
              }
            }
            for(int k=i;k<20;k++){
              if(displayLine[k]<displayLine[i]){
                displayLine[k]+=1;
              }
            }
          }
        }
        for(int i=0;i<4;i++){
          gameArray[newObject[i][0]][newObject[i][1]]=1;
        }
        displayState++;
    }else{
        displayState=0;
        for(int i=0;i<20;i++){
          displayLine[i]=30;
        }
        needRemove=false;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void Calarray(){
  //if Nextnew is true, generate a new object
  if(Nextnew){
     CreatNewObject();
     IfneedRemove();
  }else{
    //normal situation
    if(!needRemove){
      ButtonResponse();
    }else{
       Finish=false;
       RemoveLine();
    }
    displayGameFrame();
  }
}

void displayGameFrame(){
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
  //intial displayLine
  for(int i=0;i<20;i++){
    displayLine[i]=30;
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

void DrawLCD(){
    if(!Finish){
    /////////////////////////////////////////////////////////////////////////////
    //calculate array
    Calarray();
  }
  //Draw on the LCD
  u8g.firstPage();//clear screen
  //draw new figure
  do{draw();}while(u8g.nextPage());
}

void loop(){
  FrameNum++;
  //judge button
  if(analogRead(leftButton)>500){
    button=1;
    DrawLCD();
    while(analogRead(leftButton)>500){}
    button=0;
  }else if(analogRead(rightButton)>500){
    button=2;
    DrawLCD();
    while(analogRead(rightButton)>500){}
    button=0;
  }else if(analogRead(downButton)>500){
    t=5;
  }else if(analogRead(upButton)>500){
    button=3;
    DrawLCD();
    while(analogRead(upButton)>500){}
    button=0;
  }else{t=33;}
  //display frame on LCD
  if(FrameNum==15){
    FrameNum=0;
    DrawLCD();
  }
  //delay t ms
  delay(t);
}
