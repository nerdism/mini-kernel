#ifndef KEYCODES_H
#define KEYCODES_H


/* keycodes according to scan code set 1 */

enum Pressed {
    
    /*
    INSERT    = 128,
    DELETE    = 129,
    HOME      = 130,
    END       = 131,
    PAGE_UP   = 132,
    PAGE_DOWN = 133,
    UP        = 134,
    DOWN      = 135,
    LEFT      = 136,
    RIGHT     = 137,
    HELP      = 138,
    F1        = 140,
    F2        = 141,
    F3        = 142,
    F4        = 143,
    F5        = 144,
    F6        = 145,
    F7        = 146,
    F8        = 147,
    F9        = 148,
    F10       = 149,
    F11       = 150,
    F12       = 151,
    F13       = 152,
    F14       = 153,
    F15       = 154,
    INVALID   = 255

    */


      INVALID
    , P_ESC    
    , P_DASH   
    , P_EQ     
    , P_BS     
    , P_TAB    
    , P_TAB    
    , P_LSB    
    , P_RSB    
    , P_CR     
    , P_CTRL   
    , P_Q      
    , P_W      
    , P_E      
    , P_R      
    , P_T      
    , P_Z      
    , P_I      
    , P_O      
    , P_P      
    , P_A      
    , P_S      
    , P_F
    , P_G
    , P_H
    , P_J
    , P_K
    , P_L
    , P_Y
    , P_X
    , P_C
    , P_V
    , P_B
    , P_N
    , P_M
    , P_SEMICOLON
    , P_QUOTE
    , P_TICK
    , P_LEFT_SHIFT
    , P_BACKSLASH
    , P_COMMA
    , P_DOT
    , P_SLASH
    , P_RIGHT_SHIFT
    , P_STAR
    , P_ALT
    , P_SPACE
    , P_CAPS_LOCK
    , P_F1
    , P_F2
    , P_F3
    , P_F4
    , P_F5
    , P_F6
    , P_F7
    , P_F8
    , P_F9
    , P_F10
    , P_F11
    , P_F12
    , P_NUM_LOCK
    , P_SCROLL_LOCK
    , P_1
    , P_2
    , P_3
    , P_4
    , P_5
    , P_6
    , P_7
    , P_8
    , P_9
    , P_0
    , P_KEY_0
    , P_KEY_1
    , P_KEY_2
    , P_KEY_3
    , P_KEY_4
    , P_KEY_5
    , P_KEY_6
    , P_KEY_7
    , P_KEY_8
    , P_KEY_9
    , P_KEY_MINUS
    , P_KEY_PLUS
    , P_KEY_DOT
};

enum Released {

      INVALID
    , R_ESC
    , R_DASH
    , R_EQUALS
    , R_BACKSPACE
    , R_TAB
    , R_LEFT_SQUARE_BRACKET
    , R_RIGHT_SQUARE_BRACKET
    , R_ENTER
    , R_CTRL
    , R_Q
    , R_W
    , R_E
    , R_R
    , R_T
    , R_Z
    , R_U
    , R_I
    , R_O
    , R_P
    , R_A
    , R_S
    , R_D
    , R_F
    , R_G
    , R_H
    , R_J
    , R_K
    , R_L
    , R_Y
    , R_X
    , R_C
    , R_V
    , R_B
    , R_N
    , R_M
    , R_SEMICOLON
    , R_QUOTE
    , R_TICK
    , R_LEFT_SHIFT
    , R_BACKSLASH
    , R_COMMA
    , R_DOT
    , R_SLASH
    , R_RIGHT_SHIFT
    , R_STAR
    , R_ALT
    , R_SPACE
    , R_CAPS_LOCK
    , R_F1
    , R_F2
    , R_F3
    , R_F4
    , R_F5
    , R_F6
    , R_F7
    , R_F8
    , R_F9
    , R_F10
    , R_F11
    , R_F12
    , R_NUM_LOCK
    , R_SCROLL_LOCK
    , R_1
    , R_2
    , R_3
    , R_4
    , R_5
    , R_6
    , R_7
    , R_8
    , R_9
    , R_0
    , R_KEY_0
    , R_KEY_1
    , R_KEY_2
    , R_KEY_3
    , R_KEY_4
    , R_KEY_5
    , R_KEY_6
    , R_KEY_7
    , R_KEY_8
    , R_KEY_9
    , R_KEY_MINUS
    , R_KEY_PLUS
    , R_KEY_DOT
    , MEFT_PRESS
    , MIGHT_PRESS
    , MEFT_RELEASE
    , MIGHT_RELEASE

};



enum Ascii {
    NUL       ,	
    SOH       ,	
    STX       ,	
    ETX       ,	
    EOT       ,	
    ENQ       ,	
    ACK       ,	
    BEL       ,	
    BS        , 
    HT        , 
    LF        , 
    VT        , 
    FF        , 
    CR        , 
    SO        , 
    SI        , 
    DLE       ,	
    DC1       ,	
    DC2       ,	
    DC3       ,	
    DC4       ,	
    NAK       ,	
    SYN       ,	
    ETB       ,	
    CAN       ,	
    EM        , 
    SUB       ,	
    ESC       ,	
    FS        , 
    GS        , 
    RS        , 
    US        , 
    SP        , 
    BANG      , 	
    DQUOTE    ,	
    HASH      ,	
    DOLLAR	  ,
    PERCENT	  ,
    AND       ,	
    SQUOTE    ,	
    LPAREN    ,	
    RPAREN    ,	
    STAR      ,	
    PLUS      ,	
    COMMA     ,	
    MINUS     ,	
    DOT       ,	
    SLASH     ,	
    AS_0	  ,	
    AS_1      ,	
    AS_2      ,	
    AS_3      ,	
    AS_4      ,	
    AS_5      ,	
    AS_6      ,	
    AS_7      ,	
    AS_8      ,	
    AS_9      ,	
    COLON     ,	
    SEMICOLON ,
    LARROW    ,	
    EQUALS    ,	
    RARROW    ,	
    QUESTION  ,	
    AT        ,	
    A         ,  	
    B         ,  	
    C         ,  	
    D         ,  	
    E         ,  	
    F         ,  	
    G         ,  	
    H         ,  	
    I         ,  	
    J         ,  	
    K         ,  	
    L         ,  	
    M         ,  	
    N         ,  	
    O         ,  	
    P         ,  	
    Q         ,  	
    R         ,  	
    S         ,  	
    T         ,  	
    U         ,  	
    V         ,  	
    W         ,  	
    X         ,  	
    Y         ,  	
    Z         ,  	
    LBRACKET  ,	
    BACKSLASH ,	
    RBRACKET  ,	
    CARET     ,	
    _		
    BACKTICK  ,	
    a	      ,     	
    b	      ,
    c	      ,	
    d	      ,	
    e	      ,	
    f	      ,	
    g	      ,	
    h	      ,	
    i	      ,	
    j	      ,	
    k	      ,	
    l	      ,	
    m	      ,	
    n	      ,	
    o	      ,	
    p	      ,	
    q	      ,	
    r	      ,	
    s	      ,	
    t	      ,	
    u	      ,	
    v	      ,	
    w	      ,	
    x	      ,	
    y	      ,	
    z	      ,	
    LBRACE    ,	
    PIPE      ,  
    RBRACE    ,	
    TILDE     ,  
    DEL	     

};

enum Keycodes p_keys[128] = {
    
};



#endif
