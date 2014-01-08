//
//  SocketIOTransportWebsocket.h
//  v0.4.1 ARC
//
//  based on
//  socketio-cocoa https://github.com/fpotter/socketio-cocoa
//  by Fred Potter <fpotter@pieceable.com>
//
//  using
//  https://github.com/square/SocketRocket
//  https://github.com/stig/json-framework/
//
//  reusing some parts of
//  /socket.io/socket.io.js
//
//  Created by Philipp Kyeck http://beta-interactive.de
//
//  Updated by
//    samlown   https://github.com/samlown
//    kayleg    https://github.com/kayleg
//    taiyangc  https://github.com/taiyangc
//

#import <Foundation/Foundation.h>

#import "TL_SRWebSocket.h"
#import "TL_SocketIOTransport.h"

@interface TL_SocketIOTransportWebsocket : NSObject <TL_SocketIOTransport, TL_SRWebSocketDelegate>
{
    TL_SRWebSocket *_webSocket;
}

@property (nonatomic, weak) id <TL_SocketIOTransportDelegate> delegate;

@end
