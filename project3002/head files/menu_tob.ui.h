<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>Menu</class>
 <widget class="QMainWindow" name="Menu">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>800</width>
    <height>600</height>
   </rect>
  </property>
  <property name="windowTitle">
   <string>菜品管理</string>
  </property>
  <widget class="QWidget" name="centralwidget">
   <layout class="QVBoxLayout" name="verticalLayout">
    <!-- 表格展示部分 -->
    <item>
     <widget class="QTableView" name="tableView"/>
    </item>
    
    <!-- 表单和按钮部分 -->
    <item>
     <layout class="QHBoxLayout" name="horizontalLayout">
      <!-- 左侧表单 -->
      <item>
       <layout class="QFormLayout" name="formLayout">
        <item row="0" column="0">
         <widget class="QLabel" name="label_id">
          <property name="text">
           <string>ID</string>
          </property>
         </widget>
        </item>
        <item row="0" column="1">
         <widget class="QLineEdit" name="id"/>
        </item>
        <item row="1" column="0">
         <widget class="QLabel" name="label_name">
          <property name="text">
           <string>名称</string>
          </property>
         </widget>
        </item>
        <item row="1" column="1">
         <widget class="QLineEdit" name="name"/>
        </item>
        <item row="2" column="0">
         <widget class="QLabel" name="label_price">
          <property name="text">
           <string>价格</string>
          </property>
         </widget>
        </item>
        <item row="2" column="1">
         <widget class="QSpinBox" name="price"/>
        </item>
        <item row="3" column="0">
         <widget class="QLabel" name="label_introduction">
          <property name="text">
           <string>介绍</string>
          </property>
         </widget>
        </item>
        <item row="3" column="1">
         <widget class="QTextEdit" name="introduction"/>
        </item>
        <item row="4" column="0">
         <widget class="QLabel" name="label_photo">
          <property name="text">
           <string>图片路径</string>
          </property>
         </widget>
        </item>
        <item row="4" column="1">
         <widget class="QTextEdit" name="photo"/>
        </item>
       </layout>
      </item>
      
      <!-- 右侧按钮 -->
      <item>
       <layout class="QVBoxLayout" name="verticalLayoutButtons">
        <item>
         <widget class="QPushButton" name="pushButton">
          <property name="text">
           <string>添加</string>
          </property>
         </widget>
        </item>
        <item>
         <widget class="QPushButton" name="pushButton_2">
          <property name="text">
           <string>刷新</string>
          </property>
         </widget>
        </item>
        <item>
         <widget class="QPushButton" name="pushButton_3">
          <property name="text">
           <string>删除</string>
          </property>
         </widget>
        </item>
        <item>
         <widget class="QPushButton" name="pushButton_4">
          <property name="text">
           <string>修改</string>
          </property>
         </widget>
        </item>
       </layout>
      </item>
     </layout>
    </item>
   </layout>
  </widget>
  <widget class="QMenuBar" name="menubar"/>
  <widget class="QStatusBar" name="statusbar"/>
 </widget>
</ui>