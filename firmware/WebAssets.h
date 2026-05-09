#ifndef HUAMING_WEB_ASSETS_H
#define HUAMING_WEB_ASSETS_H

#include <Arduino.h>

const char INDEX_HTML[] PROGMEM = R"HUAMING_ASSET(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
  <meta name="theme-color" content="#00c853">
  <title>华鸣智能桌牌</title>
  <link rel="stylesheet" href="style.css">
</head>
<body>
  <main class="app-shell">
    <section class="hero-card" aria-labelledby="app-title">
      <div class="brand-mark" aria-hidden="true">
        <span class="brand-pulse"></span>
        <span>HM</span>
      </div>
      <div>
        <p class="eyebrow">ESP8266 · 四色墨水屏</p>
        <h1 id="app-title">华鸣智能桌牌</h1>
        <p class="subtitle">手机端实时生成 240×416 桌牌图像，并上传到设备热点。</p>
      </div>
    </section>

    <section class="panel preview-panel" aria-labelledby="preview-title">
      <div class="section-heading">
        <div>
          <p class="eyebrow">Live Preview</p>
          <h2 id="preview-title">四色预览</h2>
        </div>
        <span class="size-badge">240×416</span>
      </div>

      <div class="canvas-frame">
        <canvas id="previewCanvas" width="240" height="416" aria-label="桌牌图像预览"></canvas>
      </div>

      <div class="palette" aria-label="墨水屏四色色板">
        <span><i class="swatch swatch-white"></i>白</span>
        <span><i class="swatch swatch-black"></i>黑</span>
        <span><i class="swatch swatch-red"></i>红</span>
        <span><i class="swatch swatch-yellow"></i>黄</span>
      </div>
    </section>

    <section class="panel form-panel" aria-labelledby="form-title">
      <div class="section-heading">
        <div>
          <p class="eyebrow">Deskplate Data</p>
          <h2 id="form-title">桌牌信息</h2>
        </div>
      </div>

      <form id="deskplateForm" autocomplete="off">
        <label>
          <span>姓名</span>
          <input id="nameInput" name="name" type="text" maxlength="12" placeholder="请输入姓名" value="张华鸣">
        </label>

        <label>
          <span>公司</span>
          <input id="companyInput" name="company" type="text" maxlength="28" placeholder="请输入公司" value="华鸣智能科技有限公司">
        </label>

        <label>
          <span>职位</span>
          <input id="titleInput" name="title" type="text" maxlength="22" placeholder="请输入职位" value="产品总监">
        </label>

        <button id="uploadButton" class="primary-button" type="button">
          <span class="button-glow" aria-hidden="true"></span>
          上传到桌牌
        </button>
      </form>

      <div class="status-card" role="status" aria-live="polite">
        <span id="statusDot" class="status-dot"></span>
        <p id="statusText">将手机连接到 Huaming-Deskplate 热点后点击上传。</p>
      </div>

      <dl class="tech-list">
        <div>
          <dt>上传地址</dt>
          <dd>http://192.168.4.1/upload</dd>
        </div>
        <div>
          <dt>字段名</dt>
          <dd>image</dd>
        </div>
        <div>
          <dt>Bin 大小</dt>
          <dd><span id="binSizeText">24960</span> bytes</dd>
        </div>
      </dl>
    </section>
  </main>

  <script src="app.js" defer></script>
</body>
</html>

)HUAMING_ASSET";

const char STYLE_CSS[] PROGMEM = R"HUAMING_ASSET(
:root {
  color-scheme: dark;
  --bg: #06110d;
  --panel: rgba(8, 34, 25, 0.86);
  --panel-strong: rgba(9, 47, 35, 0.94);
  --line: rgba(0, 255, 156, 0.22);
  --green: #00e676;
  --green-soft: #69f0ae;
  --green-deep: #00a85a;
  --text: #effff7;
  --muted: #92b9a7;
  --danger: #ff5252;
  --yellow: #ffd740;
  --shadow: 0 24px 60px rgba(0, 0, 0, 0.42);
}

* {
  box-sizing: border-box;
}

html {
  min-height: 100%;
  background: var(--bg);
}

body {
  min-height: 100vh;
  margin: 0;
  font-family: Inter, "PingFang SC", "Microsoft YaHei", system-ui, -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;
  color: var(--text);
  background:
    radial-gradient(circle at 20% 0%, rgba(0, 230, 118, 0.24), transparent 34rem),
    radial-gradient(circle at 100% 20%, rgba(0, 200, 83, 0.18), transparent 26rem),
    linear-gradient(145deg, #06110d 0%, #061912 55%, #03100b 100%);
}

body::before {
  position: fixed;
  inset: 0;
  z-index: -1;
  content: "";
  background-image:
    linear-gradient(rgba(105, 240, 174, 0.055) 1px, transparent 1px),
    linear-gradient(90deg, rgba(105, 240, 174, 0.055) 1px, transparent 1px);
  background-size: 22px 22px;
  mask-image: linear-gradient(to bottom, rgba(0, 0, 0, 0.95), transparent 84%);
}

button,
input {
  font: inherit;
}

.app-shell {
  width: min(100%, 520px);
  min-height: 100vh;
  margin: 0 auto;
  padding: max(18px, env(safe-area-inset-top)) 16px max(24px, env(safe-area-inset-bottom));
}

.hero-card,
.panel {
  border: 1px solid var(--line);
  border-radius: 26px;
  background: linear-gradient(160deg, var(--panel-strong), rgba(3, 20, 14, 0.88));
  box-shadow: var(--shadow), inset 0 1px 0 rgba(255, 255, 255, 0.08);
  backdrop-filter: blur(18px);
}

.hero-card {
  display: grid;
  grid-template-columns: 58px 1fr;
  gap: 14px;
  align-items: center;
  padding: 18px;
}

.brand-mark {
  position: relative;
  display: grid;
  width: 58px;
  height: 58px;
  place-items: center;
  overflow: hidden;
  border: 1px solid rgba(105, 240, 174, 0.42);
  border-radius: 18px;
  color: #04120c;
  font-weight: 900;
  letter-spacing: -0.08em;
  background: linear-gradient(135deg, var(--green-soft), var(--green));
  box-shadow: 0 0 34px rgba(0, 230, 118, 0.38);
}

.brand-pulse {
  position: absolute;
  inset: -28%;
  background: conic-gradient(from 0deg, transparent, rgba(255, 255, 255, 0.8), transparent 30%);
  animation: spin 4s linear infinite;
}

.brand-mark span:last-child {
  position: relative;
}

@keyframes spin {
  to { transform: rotate(1turn); }
}

h1,
h2,
p {
  margin: 0;
}

h1 {
  font-size: clamp(1.85rem, 9vw, 2.55rem);
  line-height: 1.04;
  letter-spacing: -0.08em;
}

h2 {
  font-size: 1.08rem;
}

.eyebrow {
  margin-bottom: 5px;
  color: var(--green-soft);
  font-size: 0.72rem;
  font-weight: 800;
  letter-spacing: 0.14em;
  text-transform: uppercase;
}

.subtitle {
  margin-top: 8px;
  color: var(--muted);
  font-size: 0.9rem;
  line-height: 1.55;
}

.panel {
  margin-top: 16px;
  padding: 16px;
}

.section-heading {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
  margin-bottom: 14px;
}

.size-badge {
  padding: 8px 10px;
  border: 1px solid rgba(0, 230, 118, 0.32);
  border-radius: 999px;
  color: var(--green-soft);
  background: rgba(0, 230, 118, 0.1);
  font-size: 0.76rem;
  font-weight: 800;
}

.canvas-frame {
  display: grid;
  justify-content: center;
  padding: 14px;
  border: 1px solid rgba(105, 240, 174, 0.18);
  border-radius: 22px;
  background:
    linear-gradient(135deg, rgba(0, 230, 118, 0.12), transparent),
    rgba(0, 0, 0, 0.22);
}

#previewCanvas {
  display: block;
  width: min(240px, calc(100vw - 76px));
  height: auto;
  border-radius: 10px;
  background: #fff;
  box-shadow: 0 18px 36px rgba(0, 0, 0, 0.4), 0 0 0 1px rgba(255, 255, 255, 0.22);
  image-rendering: auto;
}

.palette {
  display: grid;
  grid-template-columns: repeat(4, 1fr);
  gap: 8px;
  margin-top: 12px;
}

.palette span {
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 6px;
  min-height: 34px;
  border: 1px solid rgba(105, 240, 174, 0.16);
  border-radius: 14px;
  color: var(--muted);
  background: rgba(255, 255, 255, 0.045);
  font-size: 0.82rem;
  font-weight: 700;
}

.swatch {
  width: 13px;
  height: 13px;
  border-radius: 50%;
  box-shadow: 0 0 0 1px rgba(255, 255, 255, 0.22);
}

.swatch-white { background: #fff; }
.swatch-black { background: #000; }
.swatch-red { background: #d70000; }
.swatch-yellow { background: #ffd400; }

form {
  display: grid;
  gap: 12px;
}

label {
  display: grid;
  gap: 7px;
  color: var(--muted);
  font-size: 0.84rem;
  font-weight: 800;
}

input {
  width: 100%;
  min-height: 50px;
  padding: 0 14px;
  border: 1px solid rgba(105, 240, 174, 0.18);
  border-radius: 16px;
  outline: 0;
  color: var(--text);
  background: rgba(0, 0, 0, 0.26);
  transition: border-color 160ms ease, box-shadow 160ms ease, background 160ms ease;
}

input:focus {
  border-color: var(--green);
  background: rgba(0, 0, 0, 0.34);
  box-shadow: 0 0 0 4px rgba(0, 230, 118, 0.14);
}

.primary-button {
  position: relative;
  min-height: 54px;
  margin-top: 4px;
  overflow: hidden;
  border: 0;
  border-radius: 18px;
  color: #02120b;
  background: linear-gradient(135deg, var(--green-soft), var(--green));
  box-shadow: 0 16px 34px rgba(0, 200, 83, 0.3);
  font-weight: 900;
  letter-spacing: 0.04em;
}

.primary-button:disabled {
  cursor: wait;
  opacity: 0.72;
}

.button-glow {
  position: absolute;
  inset: 0;
  transform: translateX(-100%);
  background: linear-gradient(90deg, transparent, rgba(255, 255, 255, 0.54), transparent);
  animation: sweep 2.7s ease-in-out infinite;
}

@keyframes sweep {
  45%, 100% { transform: translateX(100%); }
}

.status-card {
  display: flex;
  gap: 10px;
  align-items: flex-start;
  margin-top: 14px;
  padding: 12px;
  border: 1px solid rgba(105, 240, 174, 0.16);
  border-radius: 16px;
  color: var(--muted);
  background: rgba(0, 0, 0, 0.22);
  font-size: 0.82rem;
  line-height: 1.5;
}

.status-dot {
  width: 9px;
  height: 9px;
  margin-top: 5px;
  flex: 0 0 auto;
  border-radius: 50%;
  background: var(--green);
  box-shadow: 0 0 18px var(--green);
}

.status-card.is-error .status-dot {
  background: var(--danger);
  box-shadow: 0 0 18px var(--danger);
}

.status-card.is-busy .status-dot {
  background: var(--yellow);
  box-shadow: 0 0 18px var(--yellow);
}

.tech-list {
  display: grid;
  gap: 8px;
  margin: 14px 0 0;
}

.tech-list div {
  display: flex;
  justify-content: space-between;
  gap: 12px;
  padding: 9px 2px;
  border-bottom: 1px solid rgba(105, 240, 174, 0.1);
}

.tech-list dt,
.tech-list dd {
  margin: 0;
  font-size: 0.78rem;
}

.tech-list dt {
  color: var(--muted);
}

.tech-list dd {
  color: var(--green-soft);
  font-weight: 800;
  text-align: right;
  word-break: break-all;
}

@media (min-width: 460px) {
  .app-shell {
    padding-inline: 22px;
  }

  .panel,
  .hero-card {
    border-radius: 30px;
  }
}

)HUAMING_ASSET";

const char APP_JS[] PROGMEM = R"HUAMING_ASSET(
const CANVAS_WIDTH = 240;
const CANVAS_HEIGHT = 416;
const BIN_SIZE = (CANVAS_WIDTH * CANVAS_HEIGHT) / 4;
const UPLOAD_URL = 'http://192.168.4.1/upload';

// Firmware protocol codes consumed by Color_get():
// 0 -> white, 1 -> yellow, 2 -> red, 3 -> black. The display driver
// remaps these protocol codes to the panel-native 2-bit values before SPI.
const EPD_PALETTE = [
  { name: 'white', code: 0, rgb: [255, 255, 255], css: '#ffffff' },
  { name: 'yellow', code: 1, rgb: [255, 212, 0], css: '#ffd400' },
  { name: 'red', code: 2, rgb: [215, 0, 0], css: '#d70000' },
  { name: 'black', code: 3, rgb: [0, 0, 0], css: '#000000' },
];

const canvas = document.querySelector('#previewCanvas');
const ctx = canvas.getContext('2d', { willReadFrequently: true });
const nameInput = document.querySelector('#nameInput');
const companyInput = document.querySelector('#companyInput');
const titleInput = document.querySelector('#titleInput');
const uploadButton = document.querySelector('#uploadButton');
const statusCard = document.querySelector('.status-card');
const statusText = document.querySelector('#statusText');
const binSizeText = document.querySelector('#binSizeText');

binSizeText.textContent = BIN_SIZE.toString();

function setStatus(message, type = 'ready') {
  statusText.textContent = message;
  statusCard.classList.toggle('is-error', type === 'error');
  statusCard.classList.toggle('is-busy', type === 'busy');
}

function safeText(value, fallback) {
  const normalized = value.trim();
  return normalized || fallback;
}

function drawRoundRect(context, x, y, width, height, radius) {
  const r = Math.min(radius, width / 2, height / 2);
  context.beginPath();
  context.moveTo(x + r, y);
  context.arcTo(x + width, y, x + width, y + height, r);
  context.arcTo(x + width, y + height, x, y + height, r);
  context.arcTo(x, y + height, x, y, r);
  context.arcTo(x, y, x + width, y, r);
  context.closePath();
}

function fillTextFit(context, text, x, y, maxWidth, maxFontSize, minFontSize, weight = 700) {
  let fontSize = maxFontSize;
  do {
    context.font = `${weight} ${fontSize}px "PingFang SC", "Microsoft YaHei", Arial, sans-serif`;
    if (context.measureText(text).width <= maxWidth || fontSize <= minFontSize) {
      break;
    }
    fontSize -= 1;
  } while (fontSize >= minFontSize);

  context.fillText(text, x, y);
  return fontSize;
}

function drawPreview() {
  const name = safeText(nameInput.value, '姓名');
  const company = safeText(companyInput.value, '公司名称');
  const title = safeText(titleInput.value, '职位');

  ctx.imageSmoothingEnabled = false;
  ctx.fillStyle = EPD_PALETTE[0].css;
  ctx.fillRect(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT);

  ctx.fillStyle = EPD_PALETTE[3].css;
  ctx.fillRect(0, 0, CANVAS_WIDTH, 10);
  ctx.fillRect(0, CANVAS_HEIGHT - 10, CANVAS_WIDTH, 10);

  ctx.fillStyle = EPD_PALETTE[2].css;
  ctx.fillRect(0, 10, CANVAS_WIDTH, 30);
  ctx.fillRect(0, CANVAS_HEIGHT - 40, CANVAS_WIDTH, 30);

  ctx.fillStyle = EPD_PALETTE[1].css;
  ctx.fillRect(18, 58, 204, 8);
  ctx.fillRect(18, 347, 204, 8);
  ctx.fillRect(18, 76, 16, 16);
  ctx.fillRect(206, 76, 16, 16);
  ctx.fillRect(18, 324, 16, 16);
  ctx.fillRect(206, 324, 16, 16);

  ctx.strokeStyle = EPD_PALETTE[3].css;
  ctx.lineWidth = 3;
  drawRoundRect(ctx, 13, 52, 214, 312, 12);
  ctx.stroke();

  ctx.fillStyle = EPD_PALETTE[3].css;
  ctx.textAlign = 'center';
  ctx.textBaseline = 'middle';
  fillTextFit(ctx, company, CANVAS_WIDTH / 2, 112, 190, 22, 12, 800);

  ctx.fillStyle = EPD_PALETTE[2].css;
  fillTextFit(ctx, name, CANVAS_WIDTH / 2, 205, 202, 58, 28, 900);

  ctx.fillStyle = EPD_PALETTE[3].css;
  fillTextFit(ctx, title, CANVAS_WIDTH / 2, 277, 178, 30, 16, 800);

  ctx.fillStyle = EPD_PALETTE[1].css;
  drawRoundRect(ctx, 54, 304, 132, 26, 13);
  ctx.fill();

  ctx.fillStyle = EPD_PALETTE[3].css;
  ctx.font = '800 13px Arial, sans-serif';
  ctx.fillText('HUAMING AI PLATE', CANVAS_WIDTH / 2, 317);

  ctx.fillStyle = EPD_PALETTE[0].css;
  ctx.font = '900 15px "PingFang SC", "Microsoft YaHei", Arial, sans-serif';
  ctx.fillText('华鸣智能桌牌', CANVAS_WIDTH / 2, 25);
  ctx.fillText('E-PAPER READY', CANVAS_WIDTH / 2, CANVAS_HEIGHT - 25);

  quantizeCanvasToPalette();
}

function quantizeCanvasToPalette() {
  const image = ctx.getImageData(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT);

  for (let index = 0; index < image.data.length; index += 4) {
    const code = nearestPaletteCode(
      image.data[index],
      image.data[index + 1],
      image.data[index + 2],
      image.data[index + 3],
    );
    const color = EPD_PALETTE.find((item) => item.code === code);
    image.data[index] = color.rgb[0];
    image.data[index + 1] = color.rgb[1];
    image.data[index + 2] = color.rgb[2];
    image.data[index + 3] = 255;
  }

  ctx.putImageData(image, 0, 0);
}

function nearestPaletteCode(r, g, b, alpha) {
  if (alpha < 128) {
    return 0;
  }

  let nearest = EPD_PALETTE[0];
  let bestDistance = Number.POSITIVE_INFINITY;

  for (const color of EPD_PALETTE) {
    const [pr, pg, pb] = color.rgb;
    const distance = ((r - pr) ** 2) + ((g - pg) ** 2) + ((b - pb) ** 2);
    if (distance < bestDistance) {
      bestDistance = distance;
      nearest = color;
    }
  }

  return nearest.code;
}

function getUploadMode() {
  const targetUrl = new URL(UPLOAD_URL, window.location.href);

  if (window.location.protocol === 'file:' || window.location.origin !== targetUrl.origin) {
    return 'no-cors';
  }

  return 'cors';
}

function isOpaqueUpload(response) {
  return response.type === 'opaque';
}

function canvasToBin() {
  const { data } = ctx.getImageData(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT);
  const bin = new Uint8Array(BIN_SIZE);
  let byteIndex = 0;
  let packedByte = 0;

  for (let pixelIndex = 0; pixelIndex < CANVAS_WIDTH * CANVAS_HEIGHT; pixelIndex += 1) {
    const dataIndex = pixelIndex * 4;
    const code = nearestPaletteCode(data[dataIndex], data[dataIndex + 1], data[dataIndex + 2], data[dataIndex + 3]);
    const shift = 6 - ((pixelIndex % 4) * 2);
    packedByte |= code << shift;

    if (pixelIndex % 4 === 3) {
      bin[byteIndex] = packedByte;
      byteIndex += 1;
      packedByte = 0;
    }
  }

  return bin;
}

async function uploadToDeskplate() {
  drawPreview();
  const bin = canvasToBin();

  if (bin.byteLength !== BIN_SIZE) {
    setStatus(`生成失败：bin 文件大小为 ${bin.byteLength} bytes，应为 ${BIN_SIZE} bytes。`, 'error');
    return;
  }

  const blob = new Blob([bin], { type: 'application/octet-stream' });
  const formData = new FormData();
  formData.append('image', blob, 'huaming-deskplate.bin');

  uploadButton.disabled = true;
  setStatus(`正在上传 ${BIN_SIZE} bytes 到桌牌，请保持连接 Huaming-Deskplate 热点…`, 'busy');

  try {
    const response = await fetch(UPLOAD_URL, {
      method: 'POST',
      mode: getUploadMode(),
      body: formData,
    });

    if (!isOpaqueUpload(response) && !response.ok) {
      throw new Error(`HTTP ${response.status}`);
    }

    if (isOpaqueUpload(response)) {
      setStatus('上传请求已发送，桌牌收到后会刷新墨水屏。');
    } else {
      setStatus('上传成功，桌牌正在刷新墨水屏。');
    }
  } catch (error) {
    setStatus(`上传失败：${error.message}。请确认手机已连接设备热点，地址为 192.168.4.1。`, 'error');
  } finally {
    uploadButton.disabled = false;
  }
}

[nameInput, companyInput, titleInput].forEach((input) => {
  input.addEventListener('input', drawPreview);
});

uploadButton.addEventListener('click', uploadToDeskplate);

drawPreview();

)HUAMING_ASSET";

#endif  // HUAMING_WEB_ASSETS_H
