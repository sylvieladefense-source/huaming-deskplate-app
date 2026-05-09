# 华鸣智能桌牌 Android App

这是一个可直接用 Android Studio 打开的 Android 工程。App 启动后会全屏打开桌牌热点服务地址：

```text
http://192.168.4.1
```

## 功能

- App 名称：华鸣智能桌牌
- 使用原生 `WebView` 加载 `http://192.168.4.1`
- 已开启 JavaScript 和 DOM Storage
- 支持明文 HTTP 访问局域网设备
- 手机全屏沉浸式显示，隐藏状态栏和导航栏
- 连接失败时显示“请连接 Huaming-Deskplate WiFi 后重试。”并提供重试按钮

## 用 Android Studio 构建 APK

1. 安装 Android Studio，并通过 SDK Manager 安装：
   - Android SDK Platform 36
   - Android SDK Build-Tools
   - Android SDK Platform-Tools
2. 在 Android Studio 中选择 **File > Open**，打开本目录：`android-app`。
3. 等待 Gradle Sync 完成。
4. 构建调试 APK：选择 **Build > Build Bundle(s) / APK(s) > Build APK(s)**。
5. 构建完成后，APK 通常位于：

```text
android-app/app/build/outputs/apk/debug/app-debug.apk
```

## 命令行构建调试 APK

命令行方式需要本机已安装 Gradle（建议 8.13 或更高版本）并配置 Android SDK。然后在仓库根目录执行：

```bash
cd android-app
gradle assembleDebug
```

输出 APK：

```text
app/build/outputs/apk/debug/app-debug.apk
```

## 构建发布 APK

如需发布版 APK，请先创建签名密钥：

```bash
keytool -genkeypair \
  -v \
  -keystore huaming-deskplate-release.jks \
  -alias huaming-deskplate \
  -keyalg RSA \
  -keysize 2048 \
  -validity 10000
```

然后在 `android-app` 目录创建 `keystore.properties`：

```properties
storeFile=huaming-deskplate-release.jks
storePassword=你的密钥库密码
keyAlias=huaming-deskplate
keyPassword=你的密钥密码
```

当前工程默认只配置了 debug 构建。如需正式发布，可在 `app/build.gradle` 中增加 `signingConfigs.release` 和 `buildTypes.release` 后，在 `android-app` 目录执行：

```bash
gradle assembleRelease
```

## 安装到手机

1. 手机连接 `Huaming-Deskplate` WiFi。
2. 打开开发者选项和 USB 调试。
3. 使用 USB 连接电脑后执行：

```bash
adb install -r app/build/outputs/apk/debug/app-debug.apk
```

4. 启动“华鸣智能桌牌”。如果提示无法连接，请确认手机已连接 `Huaming-Deskplate` WiFi，且设备服务地址为 `http://192.168.4.1`。

## 可选：生成 Gradle Wrapper

如果团队希望统一 Gradle 版本，可在网络可访问 `services.gradle.org` 的环境中执行：

```bash
cd android-app
gradle wrapper --gradle-version 8.13 --distribution-type bin
```

之后即可用 `./gradlew assembleDebug` 替代 `gradle assembleDebug`。
