import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:flcompositor/flcompositor.dart';

void main() {
  const MethodChannel channel = MethodChannel('flcompositor');

  TestWidgetsFlutterBinding.ensureInitialized();

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      return '42';
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });

  test('getPlatformVersion', () async {
    expect(await Flcompositor.platformVersion, '42');
  });
}
