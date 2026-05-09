#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>

#include "Display_EPD_W21_spi.h"
#include "Display_EPD_W21.h"
#include "WebAssets.h"

ESP8266WebServer server(80);

#define IMG_SIZE ALLSCREEN_BYTES

uint8_t imageBuffer[IMG_SIZE];
volatile bool imageReady = false;
size_t uploadedSize = 0;

// 你的接线：
// BUSY -> D1
// RST  -> D4
// DC   -> D2
// CS   -> D8
// SCK  -> D5
// SDA  -> D7

void setup() {
  Serial.begin(115200);

#ifdef ESP8266
  pinMode(D1, INPUT);   // BUSY
  pinMode(D4, OUTPUT);  // RST
  pinMode(D2, OUTPUT);  // DC
  pinMode(D8, OUTPUT);  // CS
#endif

  SPI.begin();
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));

  WiFi.mode(WIFI_AP);
  WiFi.softAP("Huaming-Deskplate", "12345678");

  Serial.println();
  Serial.println("WiFi AP started");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/index.html", HTTP_GET, handleRoot);
  server.on("/style.css", HTTP_GET, handleStyleCss);
  server.on("/app.js", HTTP_GET, handleAppJs);
  server.on("/upload", HTTP_POST, handleUploadDone, handleUpload);

  server.begin();
}

void loop() {
  server.handleClient();

  if (imageReady) {
    imageReady = false;

    EPD_init();
    PIC_display(imageBuffer);
    EPD_sleep();

    Serial.println("E-paper updated.");
  }

#ifdef ESP8266
  ESP.wdtFeed();
#endif
}

void sendProgmemAsset(const char* content, const char* contentType) {
  server.sendHeader("Cache-Control", "no-cache");
  server.send_P(200, contentType, content);
}

void handleRoot() {
  sendProgmemAsset(INDEX_HTML, "text/html; charset=utf-8");
}

void handleStyleCss() {
  sendProgmemAsset(STYLE_CSS, "text/css; charset=utf-8");
}

void handleAppJs() {
  sendProgmemAsset(APP_JS, "application/javascript; charset=utf-8");
}

void handleUpload() {
  HTTPUpload& upload = server.upload();

  if (upload.status == UPLOAD_FILE_START) {
    uploadedSize = 0;
    Serial.println("Upload start");
  }

  else if (upload.status == UPLOAD_FILE_WRITE) {
    if (uploadedSize + upload.currentSize <= IMG_SIZE) {
      memcpy(imageBuffer + uploadedSize, upload.buf, upload.currentSize);
      uploadedSize += upload.currentSize;
    }
  }

  else if (upload.status == UPLOAD_FILE_END) {
    Serial.print("Upload size: ");
    Serial.println(uploadedSize);

    if (uploadedSize == IMG_SIZE) {
      imageReady = true;
    }
  }
}

void handleUploadDone() {
  if (uploadedSize == IMG_SIZE) {
    server.send(200, "text/html", "<h2>上传成功，墨水屏正在刷新</h2><a href='/'>返回</a>");
  } else {
    server.send(400, "text/html", "<h2>文件大小错误</h2><p>必须是 24960 bytes 的 .bin 文件</p><a href='/'>返回</a>");
  }
}