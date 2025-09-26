// Minimal `build.sbt` for the `nucleusrv` subproject.
// The repository root defines the multi-project layout (caravan, berkeley-hardfloat, BabyKyberAcceleratorCHISEL, nucleusrv).
// Keep this file simple to avoid duplicate project definitions when sbt is run from the repo root.

ThisBuild / scalaVersion := "2.13.10"

name := "nucleusrv"

libraryDependencies ++= Seq(
  "edu.berkeley.cs" %% "chisel3" % "3.5.6",
  "edu.berkeley.cs" %% "chiseltest" % "0.5.6" % "test",
  "org.scalatest" %% "scalatest" % "3.2.0" % "test",
  // Caravan uses play-json; keep it here so compiling this subproject standalone works.
  "com.typesafe.play" %% "play-json" % "2.9.2"
)

addCompilerPlugin("edu.berkeley.cs" % "chisel3-plugin" % "3.5.6" cross CrossVersion.full)

scalacOptions ++= Seq("-deprecation", "-feature", "-Xsource:2.13")

