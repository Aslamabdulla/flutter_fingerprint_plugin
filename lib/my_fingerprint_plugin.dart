import 'package:flutter/services.dart';

class MyFingerprintPlugin {
  static const MethodChannel _channel = MethodChannel('my_fingerprint_plugin');

  static Future<void> initialize() async {
    await _channel.invokeMethod('initialize');
  }

  static Future<void> terminate() async {
    await _channel.invokeMethod('terminate');
  }

  static Future<bool> openDevice() async {
    return await _channel.invokeMethod('openDevice');
  }

  static Future<void> closeDevice() async {
    await _channel.invokeMethod('closeDevice');
  }

  static Future<List<int>> captureFingerprint() async {
    final List<dynamic> result = await _channel.invokeMethod('captureFingerprint');
    return result.cast<int>();
  }

  static Future<int> getFingerprintQuality(List<int> fingerprint) async {
    return await _channel.invokeMethod('getFingerprintQuality', [fingerprint]);
  }

  static Future<bool> matchFingerprints(List<int> fingerprint1, List<int> fingerprint2) async {
    return await _channel.invokeMethod('matchFingerprints', [fingerprint1, fingerprint2]);
  }

  static Future<void> enableAutoOn() async {
    await _channel.invokeMethod('enableAutoOn');
  }

  static Future<void> handleAutoCapture() async {
    await _channel.invokeMethod('handleAutoCapture');
  }
}
