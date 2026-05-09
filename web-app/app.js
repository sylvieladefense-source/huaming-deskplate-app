const CANVAS_WIDTH = 240;
const CANVAS_HEIGHT = 416;
const BIN_SIZE = (CANVAS_WIDTH * CANVAS_HEIGHT) / 4;
const UPLOAD_URL = 'http://192.168.4.1/upload';

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
      body: formData,
    });

    if (!response.ok) {
      throw new Error(`HTTP ${response.status}`);
    }

    setStatus('上传成功，桌牌正在刷新墨水屏。');
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
