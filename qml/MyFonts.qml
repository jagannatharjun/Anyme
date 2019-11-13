import QtQuick 2.0

Item {
    FontLoader {
        id: varelaFont
        source: 'qrc:/font/VarelaRound-Regular.otf'
    }

    FontLoader {
        id: bigShoulder
        source: 'qrc:/font/BigShouldersText-Medium.ttf'
    }

    FontLoader {
        id: sansSerif
        source: 'qrc:/font/Montserrat-Regular.otf'
    }

    FontLoader {
        id: sansSerifBold
        source: "qrc:/font/MontserratAlternates-SemiBold.otf"
    }
    readonly property alias bigShoulder: bigShoulder
    readonly property alias sansSerif: sansSerif
    readonly property alias sansSerifBold: sansSerifBold
    readonly property alias varelaFont: varelaFont
}
