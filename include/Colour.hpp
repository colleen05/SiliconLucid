#pragma once

#include <raylib.h>
#include <rlgl.h>
#include <string>
#include <cstdint>

// Grey tones & clear
#define C_CLEAR     (Colour) {0x00, 0x00, 0x00, 0x00}
#define C_WHITE     (Colour) {0xFF, 0xFF, 0xFF, 0xFF}
#define C_LTGREY    (Colour) {0xBB, 0xBB, 0xBB, 0xFF}
#define C_GREY      (Colour) {0x7F, 0x7F, 0x7F, 0xFF}
#define C_DKGREY    (Colour) {0x33, 0x33, 0x33, 0xFF}
#define C_BLACK     (Colour) {0x00, 0x00, 0x00, 0xFF}

// Reds
#define C_LTRED     (Colour) {0xFF, 0x7F, 0x7F, 0xFF}
#define C_RED       (Colour) {0xFF, 0x00, 0x00, 0xFF}
#define C_DKRED     (Colour) {0x7F, 0x00, 0x00, 0xFF}

// Greens
#define C_LTGREEN   (Colour) {0x7F, 0xFF, 0x7F, 0xFF}
#define C_GREEN     (Colour) {0x00, 0xFF, 0x00, 0xFF}
#define C_DKGREEN   (Colour) {0x00, 0x7F, 0x00, 0xFF}

// Blues
#define C_LTBLUE    (Colour) {0x7F, 0x7F, 0xFF, 0xFF}
#define C_BLUE      (Colour) {0x00, 0x00, 0xFF, 0xFF}
#define C_DKBLUE    (Colour) {0x00, 0x00, 0x7F, 0xFF}

// Cyans
#define C_LTCYAN    (Colour) {0x7F, 0xFF, 0xFF, 0xFF}
#define C_CYAN      (Colour) {0x00, 0xFF, 0xFF, 0xFF}
#define C_DKCYAN    (Colour) {0x00, 0x7F, 0x7F, 0xFF}

// Magentas
#define C_LTMAGENTA (Colour) {0xFF, 0x7F, 0xFF, 0xFF}
#define C_MAGENTA   (Colour) {0xFF, 0x00, 0xFF, 0xFF}
#define C_DKMAGENTA (Colour) {0x7F, 0x00, 0x7F, 0xFF}
#define C_FUCHSIA   (Colour) {0xFF, 0x00, 0x7F, 0xFF}

// Yellows
#define C_YELLOW    (Colour) {0xFF, 0xFF, 0x00, 0xFF}
#define C_DKYELLOW  (Colour) {0x7F, 0x7F, 0x00, 0xFF}
#define C_ORANGE    (Colour) {0xFF, 0x7F, 0x00, 0xFF}
#define C_BROWN     (Colour) {0x7F, 0x33, 0x00, 0xFF}

// Purples
#define C_LTPURPLE  (Colour) {0xBB, 0x7F, 0xFF, 0xFF}
#define C_PURPLE    (Colour) {0x7F, 0x00, 0xFF, 0xFF}
#define C_DKPURPLE  (Colour) {0x55, 0x00, 0x7F, 0xFF}


/*! @brief Colour struct
 *  @details Contains colour information, as well as functions that allow
 *  for colour manipulation.
 */
struct Colour {
    uint8_t r;  //!< Red value
    uint8_t g;  //!< Green value
    uint8_t b;  //!< Blue value
    uint8_t a;  //!< Alpha value (opacity)

    /*! @brief Get blending result with another colour.
     *  @details Get linear interpolation with another colour.
     *  @param colour Other colour
     *  @param t Ratio of **colour** to self.
     *  @returns Blended colour.
     */
    constexpr
    Colour Lerp(const Colour colour, const float t) {
        return Colour {
            static_cast<uint8_t>(static_cast<float>(r) + t * static_cast<float>(colour.r - r)),
            static_cast<uint8_t>(static_cast<float>(g) + t * static_cast<float>(colour.g - g)),
            static_cast<uint8_t>(static_cast<float>(b) + t * static_cast<float>(colour.b - b)),
            static_cast<uint8_t>(static_cast<float>(a) + t * static_cast<float>(colour.a - a))
        };
    }

    /*! @brief Blend two colours.
     *  @details Linearly interpolate two colours.
     *  @param c1 Colour from
     *  @param c2 Colour to
     *  @param t Ratio of c1 to c2.
     */
    static Colour Lerp(Colour c1, Colour c2, const float t) {
        return c1.Lerp(c2, t);
    }

    /*! @brief Get normalised colour values
     *  @details Get colour values normalised between 0.0f and 1.0f
     *  @returns Normalised vector
     */
    constexpr
    Vector4 Normalised() {
        return Vector4 {
            static_cast<float>(r) / 255.0f,
            static_cast<float>(g) / 255.0f,
            static_cast<float>(b) / 255.0f,
            static_cast<float>(a) / 255.0f
        };
    }

    /*! @brief Get normalised values from colour
     *  @details Get normalised values of colour between 0.0f and 1.0f
     *  @param colour The colour to normalise
     *  @returns Normalised vector
     */
    static Vector4 Normalise(Colour colour) {
        return colour.Normalised();
    }

    /*! @brief Get basic struct from Colour
     *  @details Constructs a Color struct from a Colour.
     *  @note This is supplied for compatibility with raylib.
     */
    constexpr operator Color() {
        return Color{r, g, b, a};
    }
};

/*! @brief Colours namespace
 *  @details These colours are the default X11 / web colours.
 *  @see https://en.wikipedia.org/wiki/Web_colors#X11_color_names
 */
namespace Colours {
    const Colour AliceBlue            = {0xf0, 0xf8, 0xff, 0xff}; //!< AliceBlue    
    const Colour AntiqueWhite         = {0xfa, 0xeb, 0xd7, 0xff}; //!< AntiqueWhite   
    const Colour Aqua                 = {0x00, 0xff, 0xff, 0xff}; //!< Aqua      
    const Colour Aquamarine           = {0x7f, 0xff, 0xd4, 0xff}; //!< Aquamarine     
    const Colour Azure                = {0xf0, 0xff, 0xff, 0xff}; //!< Azure      
    const Colour Beige                = {0xf5, 0xf5, 0xdc, 0xff}; //!< Beige      
    const Colour Bisque               = {0xff, 0xe4, 0xc4, 0xff}; //!< Bisque      
    const Colour Black                = {0x00, 0x00, 0x00, 0xff}; //!< Black      
    const Colour BlanchedAlmond       = {0xff, 0xeb, 0xcd, 0xff}; //!< BlanchedAlmond    
    const Colour Blue                 = {0x00, 0x00, 0xff, 0xff}; //!< Blue      
    const Colour BlueViolet           = {0x8a, 0x2b, 0xe2, 0xff}; //!< BlueViolet     
    const Colour Brown                = {0xa5, 0x2a, 0x2a, 0xff}; //!< Brown      
    const Colour BurlyWood            = {0xde, 0xb8, 0x87, 0xff}; //!< BurlyWood     
    const Colour CadetBlue            = {0x5f, 0x9e, 0xa0, 0xff}; //!< CadetBlue     
    const Colour Chartreuse           = {0x7f, 0xff, 0x00, 0xff}; //!< Chartreuse     
    const Colour Chocolate            = {0xd2, 0x69, 0x1e, 0xff}; //!< Chocolate     
    const Colour Coral                = {0xff, 0x7f, 0x50, 0xff}; //!< Coral      
    const Colour CornflowerBlue       = {0x64, 0x95, 0xed, 0xff}; //!< CornflowerBlue    
    const Colour Cornsilk             = {0xff, 0xf8, 0xdc, 0xff}; //!< Cornsilk     
    const Colour Crimson              = {0xdc, 0x14, 0x3c, 0xff}; //!< Crimson     
    const Colour Cyan                 = {0x00, 0xff, 0xff, 0xff}; //!< Cyan      
    const Colour DarkBlue             = {0x00, 0x00, 0x8b, 0xff}; //!< DarkBlue     
    const Colour DarkCyan             = {0x00, 0x8b, 0x8b, 0xff}; //!< DarkCyan     
    const Colour DarkGoldenrod        = {0xb8, 0x86, 0x0b, 0xff}; //!< DarkGoldenrod    
    const Colour DarkGray             = {0xa9, 0xa9, 0xa9, 0xff}; //!< DarkGray     
    const Colour DarkGreen            = {0x00, 0x64, 0x00, 0xff}; //!< DarkGreen     
    const Colour DarkKhaki            = {0xbd, 0xb7, 0x6b, 0xff}; //!< DarkKhaki     
    const Colour DarkMagenta          = {0x8b, 0x00, 0x8b, 0xff}; //!< DarkMagenta    
    const Colour DarkOliveGreen       = {0x55, 0x6b, 0x2f, 0xff}; //!< DarkOliveGreen    
    const Colour DarkOrange           = {0xff, 0x8c, 0x00, 0xff}; //!< DarkOrange     
    const Colour DarkOrchid           = {0x99, 0x32, 0xcc, 0xff}; //!< DarkOrchid     
    const Colour DarkRed              = {0x8b, 0x00, 0x00, 0xff}; //!< DarkRed     
    const Colour DarkSalmon           = {0xe9, 0x96, 0x7a, 0xff}; //!< DarkSalmon     
    const Colour DarkSeaGreen         = {0x8f, 0xbc, 0x8f, 0xff}; //!< DarkSeaGreen    
    const Colour DarkSlateBlue        = {0x48, 0x3d, 0x8b, 0xff}; //!< DarkSlateBlue    
    const Colour DarkSlateGray        = {0x2f, 0x4f, 0x4f, 0xff}; //!< DarkSlateGray    
    const Colour DarkTurquoise        = {0x00, 0xce, 0xd1, 0xff}; //!< DarkTurquoise    
    const Colour DarkViolet           = {0x94, 0x00, 0xd3, 0xff}; //!< DarkViolet     
    const Colour DeepPink             = {0xff, 0x14, 0x93, 0xff}; //!< DeepPink     
    const Colour DeepSkyBlue          = {0x00, 0xbf, 0xff, 0xff}; //!< DeepSkyBlue    
    const Colour DimGray              = {0x69, 0x69, 0x69, 0xff}; //!< DimGray     
    const Colour DodgerBlue           = {0x1e, 0x90, 0xff, 0xff}; //!< DodgerBlue     
    const Colour FireBrick            = {0xb2, 0x22, 0x22, 0xff}; //!< FireBrick     
    const Colour FloralWhite          = {0xff, 0xfa, 0xf0, 0xff}; //!< FloralWhite    
    const Colour ForestGreen          = {0x22, 0x8b, 0x22, 0xff}; //!< ForestGreen    
    const Colour Fuchsia              = {0xff, 0x00, 0xff, 0xff}; //!< Fuchsia     
    const Colour Gainsboro            = {0xdc, 0xdc, 0xdc, 0xff}; //!< Gainsboro     
    const Colour GhostWhite           = {0xf8, 0xf8, 0xff, 0xff}; //!< GhostWhite     
    const Colour Gold                 = {0xff, 0xd7, 0x00, 0xff}; //!< Gold      
    const Colour Goldenrod            = {0xda, 0xa5, 0x20, 0xff}; //!< Goldenrod     
    const Colour Gray                 = {0x80, 0x80, 0x80, 0xff}; //!< Gray      
    const Colour Green                = {0x00, 0x80, 0x00, 0xff}; //!< Green      
    const Colour GreenYellow          = {0xad, 0xff, 0x2f, 0xff}; //!< GreenYellow    
    const Colour Honeydew             = {0xf0, 0xff, 0xf0, 0xff}; //!< Honeydew     
    const Colour HotPink              = {0xff, 0x69, 0xb4, 0xff}; //!< HotPink     
    const Colour IndianRed            = {0xcd, 0x5c, 0x5c, 0xff}; //!< IndianRed     
    const Colour Indigo               = {0x4b, 0x00, 0x82, 0xff}; //!< Indigo      
    const Colour Ivory                = {0xff, 0xff, 0xf0, 0xff}; //!< Ivory      
    const Colour Khaki                = {0xf0, 0xe6, 0x8c, 0xff}; //!< Khaki      
    const Colour Lavender             = {0xe6, 0xe6, 0xfa, 0xff}; //!< Lavender     
    const Colour LavenderBlush        = {0xff, 0xf0, 0xf5, 0xff}; //!< LavenderBlush    
    const Colour LawnGreen            = {0x7c, 0xfc, 0x00, 0xff}; //!< LawnGreen     
    const Colour LemonChiffon         = {0xff, 0xfa, 0xcd, 0xff}; //!< LemonChiffon    
    const Colour LightBlue            = {0xad, 0xd8, 0xe6, 0xff}; //!< LightBlue     
    const Colour LightCoral           = {0xf0, 0x80, 0x80, 0xff}; //!< LightCoral     
    const Colour LightCyan            = {0xe0, 0xff, 0xff, 0xff}; //!< LightCyan     
    const Colour LightGoldenrodYellow = {0xfa, 0xfa, 0xd2, 0xff}; //!< LightGoldenrodYellow 
    const Colour LightGreen           = {0x90, 0xee, 0x90, 0xff}; //!< LightGreen      
    const Colour LightGrey            = {0xd3, 0xd3, 0xd3, 0xff}; //!< LightGrey      
    const Colour LightPink            = {0xff, 0xb6, 0xc1, 0xff}; //!< LightPink      
    const Colour LightSalmon          = {0xff, 0xa0, 0x7a, 0xff}; //!< LightSalmon    
    const Colour LightSeaGreen        = {0x20, 0xb2, 0xaa, 0xff}; //!< LightSeaGreen    
    const Colour LightSkyBlue         = {0x87, 0xce, 0xfa, 0xff}; //!< LightSkyBlue    
    const Colour LightSlateGray       = {0x77, 0x88, 0x99, 0xff}; //!< LightSlateGray    
    const Colour LightSteelBlue       = {0xb0, 0xc4, 0xde, 0xff}; //!< LightSteelBlue    
    const Colour LightYellow          = {0xff, 0xff, 0xe0, 0xff}; //!< LightYellow    
    const Colour Lime                 = {0x00, 0xff, 0x00, 0xff}; //!< Lime      
    const Colour LimeGreen            = {0x32, 0xcd, 0x32, 0xff}; //!< LimeGreen     
    const Colour Linen                = {0xfa, 0xf0, 0xe6, 0xff}; //!< Linen      
    const Colour Magenta              = {0xff, 0x00, 0xff, 0xff}; //!< Magenta    
    const Colour Maroon               = {0x80, 0x00, 0x00, 0xff}; //!< Maroon      
    const Colour MediumAquamarine     = {0x66, 0xcd, 0xaa, 0xff}; //!< MediumAquamarine   
    const Colour MediumBlue           = {0x00, 0x00, 0xcd, 0xff}; //!< MediumBlue       
    const Colour MediumOrchid         = {0xba, 0x55, 0xd3, 0xff}; //!< MediumOrchid     
    const Colour MediumPurple         = {0x93, 0x70, 0xdb, 0xff}; //!< MediumPurple     
    const Colour MediumSeaGreen       = {0x3c, 0xb3, 0x71, 0xff}; //!< MediumSeaGreen     
    const Colour MediumSlateBlue      = {0x7b, 0x68, 0xee, 0xff}; //!< MediumSlateBlue   
    const Colour MediumSpringGreen    = {0x00, 0xfa, 0x9a, 0xff}; //!< MediumSpringGreen   
    const Colour MediumTurquoise      = {0x48, 0xd1, 0xcc, 0xff}; //!< MediumTurquoise   
    const Colour MediumVioletRed      = {0xc7, 0x15, 0x85, 0xff}; //!< MediumVioletRed   
    const Colour MidnightBlue         = {0x19, 0x19, 0x70, 0xff}; //!< MidnightBlue    
    const Colour MintCream            = {0xf5, 0xff, 0xfa, 0xff}; //!< MintCream      
    const Colour MistyRose            = {0xff, 0xe4, 0xe1, 0xff}; //!< MistyRose      
    const Colour Moccasin             = {0xff, 0xe4, 0xb5, 0xff}; //!< Moccasin      
    const Colour NavajoWhite          = {0xff, 0xde, 0xad, 0xff}; //!< NavajoWhite    
    const Colour Navy                 = {0x00, 0x00, 0x80, 0xff}; //!< Navy       
    const Colour OldLace              = {0xfd, 0xf5, 0xe6, 0xff}; //!< OldLace     
    const Colour Olive                = {0x80, 0x80, 0x00, 0xff}; //!< Olive      
    const Colour OliveDrab            = {0x6b, 0x8e, 0x23, 0xff}; //!< OliveDrab     
    const Colour Orange               = {0xff, 0xa5, 0x00, 0xff}; //!< Orange       
    const Colour OrangeRed            = {0xff, 0x45, 0x00, 0xff}; //!< OrangeRed     
    const Colour Orchid               = {0xda, 0x70, 0xd6, 0xff}; //!< Orchid       
    const Colour PaleGoldenrod        = {0xee, 0xe8, 0xaa, 0xff}; //!< PaleGoldenrod    
    const Colour PaleGreen            = {0x98, 0xfb, 0x98, 0xff}; //!< PaleGreen      
    const Colour PaleTurquoise        = {0xaf, 0xee, 0xee, 0xff}; //!< PaleTurquoise    
    const Colour PaleVioletRed        = {0xdb, 0x70, 0x93, 0xff}; //!< PaleVioletRed    
    const Colour PapayaWhip           = {0xff, 0xef, 0xd5, 0xff}; //!< PapayaWhip     
    const Colour PeachPuff            = {0xff, 0xda, 0xb9, 0xff}; //!< PeachPuff     
    const Colour Peru                 = {0xcd, 0x85, 0x3f, 0xff}; //!< Peru       
    const Colour Pink                 = {0xff, 0xc0, 0xcb, 0xff}; //!< Pink       
    const Colour Plum                 = {0xdd, 0xa0, 0xdd, 0xff}; //!< Plum       
    const Colour PowderBlue           = {0xb0, 0xe0, 0xe6, 0xff}; //!< PowderBlue     
    const Colour Purple               = {0x80, 0x00, 0x80, 0xff}; //!< Purple       
    const Colour Red                  = {0xff, 0x00, 0x00, 0xff}; //!< Red       
    const Colour RosyBrown            = {0xbc, 0x8f, 0x8f, 0xff}; //!< RosyBrown     
    const Colour RoyalBlue            = {0x41, 0x69, 0xe1, 0xff}; //!< RoyalBlue     
    const Colour SaddleBrown          = {0x8b, 0x45, 0x13, 0xff}; //!< SaddleBrown    
    const Colour Salmon               = {0xfa, 0x80, 0x72, 0xff}; //!< Salmon       
    const Colour SandyBrown           = {0xf4, 0xa4, 0x60, 0xff}; //!< SandyBrown     
    const Colour SeaGreen             = {0x2e, 0x8b, 0x57, 0xff}; //!< SeaGreen     
    const Colour Seashell             = {0xff, 0xf5, 0xee, 0xff}; //!< Seashell     
    const Colour Sienna               = {0xa0, 0x52, 0x2d, 0xff}; //!< Sienna       
    const Colour Silver               = {0xc0, 0xc0, 0xc0, 0xff}; //!< Silver       
    const Colour SkyBlue              = {0x87, 0xce, 0xeb, 0xff}; //!< SkyBlue     
    const Colour SlateBlue            = {0x6a, 0x5a, 0xcd, 0xff}; //!< SlateBlue     
    const Colour SlateGray            = {0x70, 0x80, 0x90, 0xff}; //!< SlateGray     
    const Colour Snow                 = {0xff, 0xfa, 0xfa, 0xff}; //!< Snow       
    const Colour SpringGreen          = {0x00, 0xff, 0x7f, 0xff}; //!< SpringGreen    
    const Colour SteelBlue            = {0x46, 0x82, 0xb4, 0xff}; //!< SteelBlue     
    const Colour Tan                  = {0xd2, 0xb4, 0x8c, 0xff}; //!< Tan       
    const Colour Teal                 = {0x00, 0x80, 0x80, 0xff}; //!< Teal       
    const Colour Thistle              = {0xd8, 0xbf, 0xd8, 0xff}; //!< Thistle     
    const Colour Tomato               = {0xff, 0x63, 0x47, 0xff}; //!< Tomato       
    const Colour Turquoise            = {0x40, 0xe0, 0xd0, 0xff}; //!< Turquoise     
    const Colour Violet               = {0xee, 0x82, 0xee, 0xff}; //!< Violet       
    const Colour Wheat                = {0xf5, 0xde, 0xb3, 0xff}; //!< Wheat       
    const Colour White                = {0xff, 0xff, 0xff, 0xff}; //!< White       
    const Colour WhiteSmoke           = {0xf5, 0xf5, 0xf5, 0xff}; //!< WhiteSmoke     
    const Colour Yellow               = {0xff, 0xff, 0x00, 0xff}; //!< Yellow      
    const Colour YellowGreen          = {0x9a, 0xcd, 0x32, 0xff}; //!< YellowGreen    
}