/*
 Navicat Premium Data Transfer

 Source Server         : stu_data
 Source Server Type    : SQLite
 Source Server Version : 3030001
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3030001
 File Encoding         : 65001

 Date: 27/12/2022 14:37:54
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
  "createDT" blob,
  PRIMARY KEY ("stud_id", "lesson_id"),
  CONSTRAINT " lesson_id" FOREIGN KEY ("lesson_id") REFERENCES "lesson_info" ("lesson_id") ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT "stud_id" FOREIGN KEY ("stud_id") REFERENCES "student_info" ("stud_id") ON DELETE NO ACTION ON UPDATE NO ACTION
);

-- ----------------------------
-- Records of grade
-- ----------------------------
INSERT INTO "grade" VALUES (1, 1, '张三', '高等数学', 90.0, NULL);
INSERT INTO "grade" VALUES (2, 1, '李四', '高等数学', 88.0, NULL);
INSERT INTO "grade" VALUES (2, 2, '李四', '大学物理', 92.0, NULL);
INSERT INTO "grade" VALUES (1, 2, '张三', '大学物理', 94.0, NULL);
INSERT INTO "grade" VALUES (1, 3, '张三', '模拟电子技术', 90.0, NULL);
INSERT INTO "grade" VALUES (2, 3, '李四', '模拟电子技术', 85.0, NULL);

-- ----------------------------
-- Table structure for lesson_info
-- ----------------------------
DROP TABLE IF EXISTS "lesson_info";
CREATE TABLE "lesson_info" (
  "lesson_id" INTEGER NOT NULL,
  "lesson_name" TEXT NOT NULL,
  "year" TEXT,
  "term" TEXT,
  "craeteDT" blob,
  PRIMARY KEY ("lesson_id")
);

-- ----------------------------
-- Records of lesson_info
-- ----------------------------
INSERT INTO "lesson_info" VALUES (1, '高等数学Ⅱ', 2022, '下学期', NULL);
INSERT INTO "lesson_info" VALUES (2, '大学物理Ⅰ', 2022, '上学期', NULL);
INSERT INTO "lesson_info" VALUES (3, '模拟电子技术', 2022, '上学期', NULL);

-- ----------------------------
-- Table structure for student_info
-- ----------------------------
DROP TABLE IF EXISTS "student_info";
CREATE TABLE "student_info" (
  "stu_name" text NOT NULL,
  "stud_id" INTEGER(6) NOT NULL,
  "class" TEXT NOT NULL,
  "performance" text(1),
  "createDT" blob,
  PRIMARY KEY ("stud_id")
);

-- ----------------------------
-- Records of student_info
-- ----------------------------
INSERT INTO "student_info" VALUES ('张三', 1, 'a01', NULL, NULL);
INSERT INTO "student_info" VALUES ('李四', 2, 'a01', NULL, NULL);
INSERT INTO "student_info" VALUES ('王五', 3, 'a02', NULL, NULL);
INSERT INTO "student_info" VALUES ('王二麻子', 4, 'a03', NULL, NULL);

-- ----------------------------
-- Table structure for users
-- ----------------------------
DROP TABLE IF EXISTS "users";
CREATE TABLE "users" (
  "user_id" text NOT NULL,
  "passwd" TEXT NOT NULL,
  "createDT" blob,
  "flag" integer(1) NOT NULL,
  PRIMARY KEY ("user_id")
);

-- ----------------------------
-- Records of users
-- ----------------------------
INSERT INTO "users" VALUES ('admin', 'admin', NULL, 1);
INSERT INTO "users" VALUES ('user', 'user', NULL, 0);

PRAGMA foreign_keys = true;
