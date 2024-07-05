// import 'package:flutter_test/flutter_test.dart';
// import 'package:my_fingerprint_plugin/my_fingerprint_plugin.dart';
// import 'package:my_fingerprint_plugin/my_fingerprint_plugin_platform_interface.dart';
// import 'package:my_fingerprint_plugin/my_fingerprint_plugin_method_channel.dart';
// import 'package:plugin_platform_interface/plugin_platform_interface.dart';

// class MockMyFingerprintPluginPlatform
//     with MockPlatformInterfaceMixin
//     implements MyFingerprintPluginPlatform {

//   @override
//   Future<String?> getPlatformVersion() => Future.value('42');
// }

// void main() {
//   final MyFingerprintPluginPlatform initialPlatform = MyFingerprintPluginPlatform.instance;

//   test('$MethodChannelMyFingerprintPlugin is the default instance', () {
//     expect(initialPlatform, isInstanceOf<MethodChannelMyFingerprintPlugin>());
//   });

//   test('getPlatformVersion', () async {
//     MyFingerprintPlugin myFingerprintPlugin = MyFingerprintPlugin();
//     MockMyFingerprintPluginPlatform fakePlatform = MockMyFingerprintPluginPlatform();
//     MyFingerprintPluginPlatform.instance = fakePlatform;

//     expect(await myFingerprintPlugin.getPlatformVersion(), '42');
//   });
// }
