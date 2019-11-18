import QtQuick 2.0

Item {
    FontLoader {
        id: varelaFont
        source: 'qrc:/font/VarelaRound-Regular.otf'
    }

    FontLoader {
        id: bigShoulderRegular
        source: 'qrc:/font/BigShouldersDisplay-Regular.ttf'
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

    FontLoader {
        id: bigShoulderLight
        source: 'qrc:/font/BigShouldersDisplay-Light.ttf'
    }

    FontLoader {
        id: oswaldMedium
        source: 'qrc:/font/Oswald-Medium.ttf'
    }

    readonly property alias bigShoulder: bigShoulder
    readonly property alias bigShoulderLight: bigShoulderLight
    readonly property alias bigShoulderRegular: bigShoulderRegular
    readonly property alias sansSerif: sansSerif
    readonly property alias sansSerifBold: sansSerifBold
    readonly property alias varelaFont: varelaFont
    readonly property alias oswaldMedium: oswaldMedium
}
