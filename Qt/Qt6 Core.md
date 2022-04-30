# Qt6 Core and Qt5 Widgets

[toc]

## Beginner

### Introduction

Qt is written in C++ and can build and run **Anywhere**.

#### **Install Qt on Ubuntu**

1. Download the Qt Online Installer from [www.qt.io](http://www.qt.io)
2. right click, allow executing file as program

```bash
$ sudo ./qt-unified-linux-x64-4.3.0-1-online.run
```

3. Input Qt account user name and password
4. Select Custom installation to avoid the huge size

1. 1. Qt 6.2.4 (Desktop gcc 64-bit; Qt 5 Compatibility Module)
   2. We don't need to install all components, can always use MaintenanceTool to add or remove components (default installation location /opt/Qt

5. Launch Qt Creator

Need to install OpenGL on machine as well

```bash
$ sudo apt-get update

$ sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev
```



#### Qt Creator

`Tools > Options > Kits`

### C++

#### Build Process

C++ build process

- Pre-processing -> Compilation -> Linking -> Binary (executable or library)

C++ Qt build process

- Preprocessor -> Qt MOC -> Compilation -> Linking -> Binary

#### How Qt uses C++

- Uses the STL
- Replicates parts of STD
- Pointers, Pointers, Pointers
- Classes and inheritance
- Templates
- Lacks error handling
- Object copies are forbidden

`QObject` is the base class of most Qt classes. `Q_OBJECT` macro enables meta-object features

### QObject parent child relationships

Qt manages the memory for us. Qt object tree in the background

QObject can not be copied

### Signals and Slots

communicate between objects even cross threads;

```c++
QObject::connect(&oSource, &Source::mySignal, &oDestination, &Destination::mySlot); // no ()
```

`slot` can be used as a normal function as well.

`signal` is a `function prototype that MOC generates code for`

### Casting

remember:

- implicit cast is C++ doing it for us
- explicit cast is us telling C++ what to do
- `static_cast` operator performs a non-polymorphic cast
- `dynamic_cast` operator ensures inheritance is good
- `reinterpret_cast` operator types to convert it to a different type

QObject cast

prefer `qobject_cast` when working with QObjects and takes away a lot of complexity

```c++
RaceCar* rcar = new RaceCar(&a);
Car* car = qobject_cast<RaceCar*>(rcar);
car->drive();
```

### Templates

`Template classes not supported by Q_OBJECT` ; again QObject can't be copied

### Qt Basic Classes

- Qt int
- QDate, QTime, QDateTime
- QString
- QByteArray
- QVariant
- QStringList
- QList/ QVector; `QVector` is an alias of `QList` in Qt6
- QMap



## Intermediate

Qt5Compat

CMake is the recommended build generator for Qt

### QObject

`sender()` special function; `Q_OBJECT` tells `MOC` to scan the class

event driven programming

### Memory Management

stack vs. heap

Parent-Child relationship in Qt (RDK Items?); recursive call; automatic deletion

```c++
child.setParent(&parent);
child.parent();
parent.children();
```

object tree; foreach



`QScopedPointer`, `QSharedPointer`



### Collections

`QList`

`QSet`: hash set, corresponding to STL `unordered_set` 

`QMap`

`QStringList`: inherit from `QList<QString>` but better; join, filter, replaceInStrings

`qDeleteAll` with QLists/QMap, remember to clear; use smart pointer `QSharedPointer`



### Views

`QxxView` means read-only in Qt. including `QStringView`, `QByteArrayView`



### Settings

Application state: `QSettings` provides persistent platform-independent application settings. save, load and repopulate

this is bit confusing - TODO

Groups of QSettings

Setting the filename

### File system

C++17 Filesystem; Boost::Filesystem

Disk > Partition/Volume > Filesystem > Dir > file > Format

Ext4



Format:

- text
- Binary: encoded

File Extensions

QIODevice

QTextStream

QDataStream; data encoding; serialization; difference of encoding and encryption



QDir/QFileInfo; be careful with directory operation

`QDir::tempPath()` 

QStorageInfo

QLockFile

`QDir::separator()`

pid: process id



### QDebug

### Encoding

- ASCII
- UTF-8; 1112064
- UTF-16: QString uses `UTF-16` by default
- Base64
- Hex

### Compression

- zip
- 7z
- Gz

compression and decompression

3rd party lib:  QuaZip

`Gz` is baked into Qt

header, to be unique

### Serialization

Serialize data to storage; de-serialize data from storage

`QDataStream`; 

### Algorithms and macros

### Working with OS

### Timers

### Basic Threading

## Advanced

# Qt5 Widgets

## Dividing into QWidgets

`QWidget` is the base class of all user interface objects.

`sigals` and `slots`; using editor vs. coding, multiple ways of connecting

- `Internal`: In the `.ui` file, `Edit Signals/Slots (F4)` directly (in Qt Creator)
- `Editor`: right click, `go to slots`
- `Connect`: e.g.

```C++
    connect(ui->pbConnect, &QPushButton::clicked, this, &Dialog::doStuff);
```



## Layouts

- Absolute layout - Don't use it
- Horizontal layout
- Vertical layout
- Grid layout
- Form layout
- Multiple layouts

use `splitters` 



## Buttons and checkboxes

From the `Property Editor`, we can actually see the actual `inheritance tree` (e.g. QPushButton < QAbstractButton < QWidget < QObject) - Nice, didn't notice this before

`checkable` / `checked` for `QPushButton`; `autoDefult`/`default`

QCheckBox, QRadioButton

`Dialog Button Box` pretty cool; no more manually create buttons

```c++
QPushButton* btn = qobject_cast<QPushButton*>(sender());
```

`qobject_cast` is pretty useful along with the `sender()` which identifies the object sending out the signal.



## Combos, lists, spinbox, and more



## Resources

## Multiple Dialogs

## QMainWindow

## Containers

## Rich Text Editor Example

## Models

## Complete Example Applications

