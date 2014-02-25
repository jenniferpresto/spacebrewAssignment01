//2014 Parsons MFA DT/Collab: spacebrew
//Assignment 1/ Cooperate with Jennifer Presto

import spacebrew.*;

String server="sandbox.spacebrew.cc";
String name="Drawing Tool_Susan";
String description ="Two people can draw toghter and save the image once they both hit the saving image";

Spacebrew sb;

int local_xPos_val = 0;
int local_yPos_val = 0;
int local_lastX_val = 0;
int local_lastY_val = 0;
int remote_xPos_val = 0;
int remote_yPos_val = 0;
int remote_lastX_val = 0;
int remote_lastY_val = 0;
String local_color = "";
String remote_color = "";

PImage gHeart;
PImage rHeart;
PImage lHeart;
PImage Heart;
float x, y;
boolean drawing = true;
boolean saving = false;
boolean reciveSaving = false;
int r = 0;
int g = 0;
int b = 0;
int remoteR = 0;
int remoteG = 0;
int remoteB = 0;

void setup() {
  size(500, 500);
  background(255);
  ellipseMode(CENTER);
  imageMode(CENTER);
  gHeart = loadImage("gHeart.png");
  rHeart = loadImage("rHeart.png");
  lHeart = loadImage("lHeart.png");
  Heart = loadImage("Heart.png");

  sb = new Spacebrew( this);
  // declare your publishers
  sb.addPublish( "heart_pressed", "boolean", true);
  sb.addPublish( "local_xPos", "range", local_xPos_val);
  sb.addPublish( "local_yPos", "range", local_yPos_val); 
  sb.addPublish( "local_lastX", "range", local_lastX_val);
  sb.addPublish( "local_lastY", "range", local_lastY_val);   
  sb.addPublish( "local_color", "string", local_color);

  // declare your subscribers
  sb.addSubscribe( "change_heart", "boolean");
  sb.addSubscribe( "remote_xPos", "range");
  sb.addSubscribe( "remote_yPos", "range");
  sb.addSubscribe( "remote_lastX", "range");
  sb.addSubscribe( "remote_lastY", "range");
  sb.addSubscribe( "remote_color", "string");
  // connect to spacebre
  sb.connect(server, name, description);
}

void draw() {

  //selecting bar
  noStroke();
  fill(240);
  rect(0, 430, width, 40);
  //black dot  
  fill(0);
  ellipse(50, 450, 30, 30);
  //red dot
  fill(255, 0, 0);
  ellipse(100, 450, 30, 30);
  //green dot
  fill(0, 255, 0);
  ellipse(150, 450, 30, 30);
  //blue dot
  fill(0, 0, 255);
  ellipse(200, 450, 30, 30);  

  //Drawing grad heart
  image(gHeart, 400, 450);

  // if I clicked, it shows up the right half heart 
  if (saving == true) {
    image(rHeart, 400, 450);
  }
  if (reciveSaving == true) {
    image(lHeart, 400, 450);
  }
  if ((saving == true)&&(reciveSaving == true)) {
    image(Heart, 400, 450);
    save("mydrawing.png");
    saving = false;
    reciveSaving = false;
    fill(255);
    rect(0, 0, width, height);
  }

  //Drawing part  
  if (mousePressed) {
    if (drawing) {
      local_lastX_val = mouseX;
      local_lastY_val = mouseY;
      drawing = false;
    }
    stroke(r, g, b);
    strokeWeight(3);
    line (local_lastX_val, local_lastY_val, mouseX, mouseY);

    //Everytime we draw, plz send to spacebrew my brush position
    sb.send("local_xPos", mouseX);
    sb.send("local_yPos", mouseY);
    sb.send("local_lastX", local_lastX_val);
    sb.send("local_lastY", local_lastY_val);
  } 
    local_lastX_val = mouseX;
    local_lastY_val = mouseY; 
  drawing = false; 

  //draw the outline for selected color  
  if ((r == 0)&&(g == 0)&&(b == 0)) {
    noFill();
    stroke(0);
    strokeWeight(3);
    ellipse(50, 450, 36, 36);
  }
  else if ((r == 255)&&(g == 0)&&(b == 0)) {
    noFill();
    stroke(0);
    strokeWeight(3);
    ellipse(100, 450, 36, 36);
  }
  else if ((r == 0)&&(g == 255)&&(b == 0)) {
    noFill();
    stroke(0);
    strokeWeight(3);
    ellipse(150, 450, 36, 36);
  }
  else if ((r == 0)&&(g == 0)&&(b == 255)) {
    noFill();
    stroke(0);
    strokeWeight(3);
    ellipse(200, 450, 36, 36);
  }
  stroke(remoteR, remoteG, remoteB);
  line (remote_lastX_val, remote_lastY_val, remote_xPos_val, remote_yPos_val);
//  println("remote_lastY_val: "+ remote_lastY_val +";" + "remote_yPos_val: " + remote_yPos_val);
}

void mouseReleased() {
  drawing = true;
}

//Changing bush color
void mouseClicked() {
  if (dist(mouseX, mouseY, 50, 450) < 15) {
    r = 0;
    g = 0;
    b = 0;
    sb.send("local_color", "black");
  }
  else if (dist(mouseX, mouseY, 100, 450) < 15) {
    r = 255;
    g = 0;
    b = 0;
    sb.send("local_color", "red");
  }
  else if (dist(mouseX, mouseY, 150, 450) < 15) {
    r = 0;
    g = 255;
    b = 0; 
    sb.send("local_color", "green");
  }
  else if (dist(mouseX, mouseY, 200, 450) < 15) {
    r = 0;
    g = 0;
    b = 255; 
    sb.send("local_color", "blue");
  }
  else if ((dist(mouseX, mouseY, 400, 450))< 40) { //clicking the grad heart
    saving = true; 
    sb.send( "heart_pressed", true);
  }
}

//What I received from spacebrew... 
void onBooleanMessage( String name, boolean value ) {
  if ((value == true)) {
    reciveSaving = true;
    println("receiving message: "+ name +" " + value);
  }
}

//if name == subscribe ->> put the value inside the name 
void onRangeMessage( String name, int value ) {
//  println("receiving message named: " + name + " and value is " + value);
  if (name.equals("remote_xPos")) {
    remote_xPos_val = value;
  }
  else if (name.equals("remote_yPos")) {
    remote_yPos_val = value;
  }
  else if (name.equals("remote_lastX")) {
    remote_lastX_val = value;
  }
  else if (name.equals("remote_lastY")) {
    remote_lastY_val = value;
  }
}

void onStringMessage( String name, String value ) {
  if (name.equals("remote_color")) {
    if (value.equals("black")) {
      remoteR = 0;
      remoteG = 0;
      remoteB = 0;
    }
    else if (value.equals("red")) {
      remoteR = 255;
      remoteG = 0;
      remoteB = 0;
    }
    else if (value.equals("green")) {
      remoteR = 0;
      remoteG = 255;
      remoteB = 0;
    }
    else if (value.equals("blue")) {
      remoteR = 0;
      remoteG = 0;
      remoteB = 255;
    }
  }
}

