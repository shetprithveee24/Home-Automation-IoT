// ignore_for_file: must_be_immutable

import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';

class SettingsPage extends StatelessWidget {
  SettingsPage({super.key});

  String introduction =
      "Our Home Automation System uses IoT to optimize lighting and door control, offering energy-efficient, customizable, and accessible solutions. Designed for the elderly and disabled, it enhances safety and convenience. Users can control lights, doors, and fans via a mobile app for a smarter, more independent living experience.";

  String aim =
      "To develop a cutting-edge smart home automation system that automates door & lighting based on occupancy, environmental factors, and user preferences, controlled through a seamless mobile application.";
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: SingleChildScrollView(
        child: Container(
          padding: const EdgeInsets.symmetric(horizontal: 15, vertical: 20),
          child: Column(
            children: [
              Column(
                children: [
                  ClipRRect(
                    borderRadius: BorderRadius.circular(15),
                    child: ExpansionTile(
                      tilePadding: EdgeInsets.all(10),
                      childrenPadding: const EdgeInsets.symmetric(
                          horizontal: 15, vertical: 10),
                      collapsedBackgroundColor: Colors.deepPurple,
                      expandedCrossAxisAlignment: CrossAxisAlignment.stretch,
                      collapsedIconColor: Colors.white,
                      backgroundColor: Colors.deepPurple,
                      shape: LinearBorder.none,
                      title: Text(
                        "Introduction",
                        style: GoogleFonts.poppins(
                          fontSize: 18,
                          color: Colors.white,
                          fontWeight: FontWeight.bold,
                        ),
                      ),
                      children: [
                        Image.asset(
                          "images/IMG_0391.JPEG",
                          width: 500,
                          height: 240,
                          fit: BoxFit.fill,
                        ),
                        const SizedBox(height: 10),
                        Text(
                          introduction,
                          style: const TextStyle(
                            color: Colors.white,
                            fontSize: 15,
                          ),
                        )
                      ],
                    ),
                  ),
                ],
              ),

              //Aims and Objectives
              const SizedBox(height: 15),
              ClipRRect(
                borderRadius: BorderRadius.circular(15),
                child: ExpansionTile(
                  tilePadding: EdgeInsets.all(10),
                  childrenPadding:
                      const EdgeInsets.symmetric(horizontal: 15, vertical: 10),
                  expandedAlignment: Alignment.centerLeft,
                  expandedCrossAxisAlignment: CrossAxisAlignment.start,
                  collapsedBackgroundColor: Colors.deepPurple,
                  collapsedIconColor: Colors.white,
                  backgroundColor: Colors.blue,
                  shape: LinearBorder.none,
                  title: Text(
                    "Aim and objectives",
                    style: GoogleFonts.poppins(
                      fontSize: 18,
                      color: Colors.white,
                      fontWeight: FontWeight.bold,
                    ),
                  ),
                  children: [
                    Row(
                      children: [
                        Image.asset(
                          "images/bullet-point.png",
                          width: 20,
                          height: 20,
                          color: Colors.white,
                        ),
                        Text(
                          "  Aim",
                          style: GoogleFonts.poppins(
                            fontSize: 16,
                            color: Colors.white,
                            fontWeight: FontWeight.bold,
                          ),
                        ),
                      ],
                    ),
                    Text(
                      aim,
                      style: GoogleFonts.poppins(
                        fontSize: 15,
                        color: Colors.white,
                      ),
                    ),
                    const SizedBox(height: 10),
                    Row(
                      children: [
                        Image.asset(
                          "images/bullet-point.png",
                          width: 20,
                          height: 20,
                          color: Colors.white,
                        ),
                        Text(
                          "  Objectives",
                          style: GoogleFonts.poppins(
                            fontSize: 16,
                            color: Colors.white,
                            fontWeight: FontWeight.bold,
                          ),
                        ),
                      ],
                    ),
                    Text(
                      "+ Automated Door Entry \n+ Automated Entry/Exit Lighting\n+ Mood-Based Lighting\n+ Eldercare\n+ Application Control",
                      style: GoogleFonts.poppins(
                        fontSize: 15,
                        color: Colors.white,
                      ),
                    ),
                  ],
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
