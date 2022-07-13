import controlP5.*; //import ControlP5 library
import meter.*;
import processing.serial.*;

Serial port;
String[] list;

Meter m, m2;

ControlP5 cp5; //create ControlP5 object
PFont font;

void setup(){ //same as arduino program
  size(1200, 1000);
  background(0);
  
  port = new Serial(this, "COM3", 9600);

  fill(120, 50, 0);
  m = new Meter(this, 150, 100);
  // Adjust font color of meter value  
  m.setTitleFontSize(20);
  m.setTitleFontName("Arial bold");
  m.setTitle("Temperature (C)");
  m.setDisplayDigitalMeterValue(true);
  
  // Meter Scale
  String[] scaleLabelsT = {"0", "10", "20", "30", "40", "50", "60", "70", "80"};
  m.setScaleLabels(scaleLabelsT);
  m.setScaleFontSize(18);
  m.setScaleFontName("Times New Roman bold");
  m.setScaleFontColor(color(200, 30, 70));
  
  m.setArcColor(color(141, 113, 178));
  m.setArcThickness(10);
  m.setMaxScaleValue(80);
  
  m.setNeedleThickness(3);
  
  m.setMinInputSignal(0);
  m.setMaxInputSignal(80);

  // A second meter for reference
  int mx = m.getMeterX();
  int my = m.getMeterY();
  int mw = m.getMeterWidth();
  
  m2 = new Meter(this, mx + mw + 20, my);
  m2.setTitleFontSize(20);
  m2.setTitleFontName("Arial bold");
  m2.setTitle("Humidity (%)");
  m2.setDisplayDigitalMeterValue(true);
  
  String[] scaleLabelsH = {"0", "10", "20", "30", "40", "50", "60", "70", "80", "90", "100"};
  m2.setScaleLabels(scaleLabelsH);
  m2.setScaleFontSize(18);
  m2.setScaleFontName("Times New Roman bold");
  m2.setScaleFontColor(color(200, 30, 70));
  
  m2.setArcColor(color(300, 113, 178));
  m2.setArcThickness(10);
  m2.setMaxScaleValue(100);
  
  m2.setNeedleThickness(3);
  
  m2.setMinInputSignal(0);
  m2.setMaxInputSignal(100);
  
  cp5 = new ControlP5(this);
  font = createFont("calibri light bold", 20);    // custom fonts for buttons and title
  
  cp5.addButton("bathroom")     //"green" is the name of button
    .setPosition(140,500)  //x and y coordinates of upper left corner of button
    .setSize(150, 70)      //(width, height)
    .setFont(font)
  ;   

  cp5.addButton("kItchen")     //"yellow" is the name of button
    .setPosition(330, 500)  //x and y coordinates of upper left corner of button
    .setSize(150, 70)      //(width, height)
    .setFont(font)
  ;

  cp5.addButton("lIvIngroom")     //"blue" is the name of button
    .setPosition(520, 500)  //x and y coordinates of upper left corner of button
    .setSize(150, 70)      //(width, height)
    .setFont(font)
  ;
  
  cp5.addButton("alloff")     //"alloff" is the name of button
    .setPosition(800, 500)  //x and y coordinates of upper left corner of button
    .setSize(150, 70)      //(width, height)
    .setFont(font)
  ;
  
}

void draw(){  //same as loop in arduino
  textSize(30);
  fill(255);
  text("HOME AUTOMATION SYSTEM", 270, 50);
  
  if (port.available() > 0) {
    String val = port.readString();
    list = split(val, ',');
    float temp = float(list[0]);
    float hum = float(list[1]);
    
    println("Temperature: " + temp + " C  " + "Humidity: " + hum + " %");
    
    m.updateMeter(int(temp));
    m2.updateMeter(int(hum));
  }
}

//lets add some functions to our buttons
//so whe you press any button, it sends perticular char over serial port

void bathroom(){
  port.write('g');
}
                                                                                                                                                                                                               
void kItchen(){
  port.write('y');
}

void lIvIngroom(){
  port.write('b');
}

void alloff(){
  port.write('f');
}
