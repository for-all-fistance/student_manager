/*
 Navicat Premium Data Transfer

 Source Server         : stu_data
 Source Server Type    : SQLite
 Source Server Version : 3030001
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3030001
 File Encoding         : 65001

 Date: 31/01/2023 15:41:00
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for grade
-- ----------------------------
DROP TABLE IF EXISTS "grade";
CREATE TABLE "grade" (
  "stud_id" INTEGER(6) NOT NULL,
  "lesson_id" INTEGER NOT NULL,
  "stu_name" TEXT,
  "lesson_name" TEXT,
  "grade" real,
  "createDT" blob
);

-- ----------------------------
-- Table structure for lesson_info
-- ----------------------------
DROP TABLE IF EXISTS "lesson_info";
CREATE TABLE "lesson_info" (
  "lesson_id" INTEGER NOT NULL,
  "lesson_name" TEXT,
  "year" text,
  "term" TEXT,
  "createDT" blob,
  "average_score" real,
  "pass_rate" real,
  "total_count" integer,
  "perfect" integer,
  "good" integer,
  "fail" integer,
  "qualified" integer,
  PRIMARY KEY ("lesson_id")
);

-- ----------------------------
-- Table structure for sqlite_sequence
-- ----------------------------
DROP TABLE IF EXISTS "sqlite_sequence";
CREATE TABLE "sqlite_sequence" (
  "name" ,
  "seq" 
);

-- ----------------------------
-- Table structure for student_info
-- ----------------------------
DROP TABLE IF EXISTS "student_info";
CREATE TABLE "student_info" (
  "stu_name" text,
  "stud_id" INTEGER(6) NOT NULL,
  "class" text,
  "performance" text,
  "createDT" blob,
  "average_score" real,
  PRIMARY KEY ("stud_id")
);

-- ----------------------------
-- Table structure for users
-- ----------------------------
DROP TABLE IF EXISTS "users";
CREATE TABLE "users" (
  "user_id" text NOT NULL,
  "passwd" TEXT NOT NULL,
  "createDT" blob,
  PRIMARY KEY ("user_id")
);

-- ----------------------------
-- Triggers structure for table grade
-- ----------------------------
CREATE TRIGGER "autofill_grade"
AFTER INSERT
ON "grade"
BEGIN
UPDATE grade SET createDT=date('now') WHERE stud_id=NEW.stud_id AND lesson_id=NEW.lesson_id;
END;

-- ----------------------------
-- Triggers structure for table lesson_info
-- ----------------------------
CREATE TRIGGER "autofill_lesson"
AFTER INSERT
ON "lesson_info"
BEGIN
UPDATE lesson_info SET createDT=date('now') WHERE lesson_id=NEW.lesson_id;
END;
CREATE TRIGGER "del_grade_following_lesson"
BEFORE DELETE
ON "lesson_info"
BEGIN
DELETE FROM grade WHERE lesson_id=OLD.lesson_id;
END;
CREATE TRIGGER "follow_lesson_name"
AFTER UPDATE
ON "lesson_info"
BEGIN
UPDATE grade SET lesson_name=NEW.lesson_name WHERE lesson_id=NEW.lesson_id;
END;

-- ----------------------------
-- Triggers structure for table student_info
-- ----------------------------
CREATE TRIGGER "autofill_student"
AFTER INSERT
ON "student_info"
BEGIN
UPDATE student_info SET createDT=date('now') WHERE stud_id=NEW.stud_id;
END;
CREATE TRIGGER "del_grade_following_student"
AFTER DELETE
ON "student_info"
BEGIN
DELETE FROM grade WHERE stud_id=OLD.stud_id;
END;
CREATE TRIGGER "follow_stu_name"
AFTER UPDATE OF "stu_name"
ON "student_info"
BEGIN
UPDATE grade SET stu_name=NEW.stu_name WHERE stud_id=NEW.stud_id;
END;

-- ----------------------------
-- Triggers structure for table users
-- ----------------------------
CREATE TRIGGER "autofill_user"
AFTER INSERT
ON "users"
BEGIN
UPDATE user SET createDT=date('now') WHERE user_id=NEW.user_id;
END;

PRAGMA foreign_keys = true;
