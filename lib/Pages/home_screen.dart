// ignore_for_file: avoid_print, must_be_immutable

import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:home_automation01/Models/top_card.dart';
import 'package:home_automation01/Models/widget.dart';
import 'package:http/http.dart' as http;

class ControlPage extends StatefulWidget {
  const ControlPage({super.key});

  @override
  State<ControlPage> createState() => _ControlPageState();
}

class _ControlPageState extends State<ControlPage> {
  final String espIp = "192.168.43.37";

  bool mainLight = false; //Main light

  bool tableLamp = false; //Table lamp

  bool isDoorOpen = false; //Door

  bool isSleep = false; //Blue light

  Future<void> sendRequest(String endpoint) async {
    final url = Uri.parse("http://$espIp$endpoint");
    try {
      final response = await http.get(url);
      if (response.statusCode == 200) {
        print("Success: ${response.body}");
      } else {
        print("Failed with status: ${response.statusCode}");
      }
    } catch (e) {
      print("Request error: $e");
    }
  }

  void mainLightControll(bool value) {
    setState(() {
      mainLight = value;
    });
    sendRequest(value ? "/led_white/on" : "/led_white/off");
  }

  void tableLampControll(bool value) {
    setState(() {
      tableLamp = value;
    });
    sendRequest(value ? "/led_orange/on" : "/led_orange/off");
  }

  void sleepingLightControll(bool value) {
    setState(() {
      isSleep = value;
    });
    sendRequest(value ? "/led_blue/on" : "/led_blue/off");
  }

  void doorControll(bool value) {
    setState(() {
      isDoorOpen = value;
    });
    sendRequest(value ? "/door/open" : "/door/close");
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color.fromARGB(243, 244, 250, 251),
      body: SafeArea(
        child: Center(
          child: Container(
            padding: const EdgeInsets.all(15),
            child: Column(
              mainAxisAlignment: MainAxisAlignment.start,
              children: [
                //Status Bar/Greeting
                Row(
                  mainAxisAlignment: MainAxisAlignment.spaceBetween,
                  children: [
                    Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        Text(
                          "Good Morning",
                          style: GoogleFonts.poppins(
                            color: Colors.grey,
                            fontSize: 18,
                          ),
                        ),
                        Row(
                          children: [
                            Text(
                              "Hey,",
                              style: GoogleFonts.poppins(
                                color: Colors.black,
                                fontSize: 22,
                                fontWeight: FontWeight.w500,
                              ),
                            ),
                            Text(
                              " Alwis👋",
                              style: GoogleFonts.poppins(
                                color: Colors.black,
                                fontSize: 22,
                                fontWeight: FontWeight.bold,
                              ),
                            ),
                          ],
                        ),
                      ],
                    ),
                    const CircleAvatar(
                      radius: 28,
                      child: Icon(
                        Icons.person,
                        size: 35,
                      ),
                    ),
                  ],
                ),

                const SizedBox(height: 20),

                const TopCard(),

                const SizedBox(height: 20),

                Column(
                  children: [
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceBetween,
                      children: [
                        MainWidget(
                          mainText: "Smart Lighting",
                          secondaryText: "Bedroom",
                          imagePath: mainLight
                              ? "images/lamp-on.png"
                              : "images/lamp-off.png",
                          secondImagePath: "images/wi-fi.png",
                          color: Colors.teal,
                          onChanged: mainLightControll,
                          isOn: mainLight,
                        ),
                        MainWidget(
                          mainText: "Study Mode",
                          secondaryText: "Study, Music",
                          imagePath: tableLamp
                              ? "images/desk-lamp.png"
                              : "images/tlamp-off.png",
                          secondImagePath: "images/wi-fi.png",
                          color: const Color.fromARGB(255, 246, 190, 38),
                          onChanged: tableLampControll,
                          isOn: tableLamp,
                        ),
                      ],
                    ),
                    const SizedBox(height: 20),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceBetween,
                      children: [
                        MainWidget(
                          mainText: "Sleeping Mode",
                          secondaryText: "Sleeping Light",
                          imagePath: isSleep
                              ? "images/sleep.png"
                              : "images/not-sleeping.png",
                          secondImagePath: "images/wi-fi.png",
                          color: const Color.fromARGB(255, 88, 180, 255),
                          onChanged: sleepingLightControll,
                          isOn: isSleep,
                        ),
                        MainWidget(
                          mainText: "Door Control",
                          secondaryText: "Smart Door",
                          imagePath: isDoorOpen
                              ? "images/door-open.png"
                              : "images/door-close.png",
                          secondImagePath: "images/wi-fi.png",
                          color: Colors.deepPurple,
                          onChanged: doorControll,
                          isOn: isDoorOpen,
                        ),
                      ],
                    ),
                  ],
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}
