//
//  SocketIO.h
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

#import "TL_SocketIOTransport.h"

@class TL_SocketIO;
@class TL_SocketIOPacket;

typedef void(^SocketIOCallback)(id argsData);

extern NSString* const TL_SocketIOError;

typedef enum {
    SocketIOServerRespondedWithInvalidConnectionData = -1,
    SocketIOServerRespondedWithDisconnect = -2,
    SocketIOHeartbeatTimeout = -3,
    SocketIOWebSocketClosed = -4,
    SocketIOTransportsNotSupported = -5,
    SocketIOHandshakeFailed = -6,
    SocketIODataCouldNotBeSend = -7
} SocketIOErrorCodes;


@protocol SocketIODelegate <NSObject>
@optional
- (void) socketIODidConnect:(TL_SocketIO *)socket;
- (void) socketIODidDisconnect:(TL_SocketIO *)socket disconnectedWithError:(NSError *)error;
- (void) socketIO:(TL_SocketIO *)socket didReceiveMessage:(TL_SocketIOPacket *)packet;
- (void) socketIO:(TL_SocketIO *)socket didReceiveJSON:(TL_SocketIOPacket *)packet;
- (void) socketIO:(TL_SocketIO *)socket didReceiveEvent:(TL_SocketIOPacket *)packet;
- (void) socketIO:(TL_SocketIO *)socket didSendMessage:(TL_SocketIOPacket *)packet;
- (void) socketIO:(TL_SocketIO *)socket onError:(NSError *)error;

// TODO: deprecated -> to be removed
- (void) socketIO:(TL_SocketIO *)socket failedToConnectWithError:(NSError *)error __attribute__((deprecated));
- (void) socketIOHandshakeFailed:(TL_SocketIO *)socket __attribute__((deprecated));
@end


@interface TL_SocketIO : NSObject <NSURLConnectionDelegate, TL_SocketIOTransportDelegate>
{
    NSString *_host;
    NSInteger _port;
    NSString *_sid;
    NSString *_endpoint;
    NSDictionary *_params;
    
    __weak id<SocketIODelegate> _delegate;
    
    NSObject <TL_SocketIOTransport> *_transport;
    
    BOOL _isConnected;
    BOOL _isConnecting;
    BOOL _useSecure;
    
    NSURLConnection *_handshake;
    
    // heartbeat
    NSTimeInterval _heartbeatTimeout;
    dispatch_source_t _timeout;
    
    NSMutableArray *_queue;
    
    // acknowledge
    NSMutableDictionary *_acks;
    NSInteger _ackCount;
    
    // http request
    NSMutableData *_httpRequestData;
    
    // get all arguments from ack? (https://github.com/pkyeck/socket.IO-objc/pull/85)
    BOOL _returnAllDataFromAck;
}

@property (nonatomic, readonly) NSString *host;
@property (nonatomic, readonly) NSInteger port;
@property (nonatomic, readonly) NSString *sid;
@property (nonatomic, readonly) NSTimeInterval heartbeatTimeout;
@property (nonatomic) BOOL useSecure;
@property (nonatomic, readonly) BOOL isConnected, isConnecting;
@property (nonatomic, weak) id<SocketIODelegate> delegate;
@property (nonatomic) BOOL returnAllDataFromAck;

- (id) initWithDelegate:(id<SocketIODelegate>)delegate;
- (void) connectToHost:(NSString *)host onPort:(NSInteger)port;
- (void) connectToHost:(NSString *)host onPort:(NSInteger)port withParams:(NSDictionary *)params;
- (void) connectToHost:(NSString *)host onPort:(NSInteger)port withParams:(NSDictionary *)params withNamespace:(NSString *)endpoint;
- (void) connectToHost:(NSString *)host onPort:(NSInteger)port withParams:(NSDictionary *)params withNamespace:(NSString *)endpoint withConnectionTimeout: (NSTimeInterval) connectionTimeout;

- (void) disconnect;
- (void) disconnectForced;

- (void) sendMessage:(NSString *)data;
- (void) sendMessage:(NSString *)data withAcknowledge:(SocketIOCallback)function;
- (void) sendJSON:(NSDictionary *)data;
- (void) sendJSON:(NSDictionary *)data withAcknowledge:(SocketIOCallback)function;
- (void) sendEvent:(NSString *)eventName withData:(id)data;
- (void) sendEvent:(NSString *)eventName withData:(id)data andAcknowledge:(SocketIOCallback)function;
- (void) sendAcknowledgement:(NSString*)pId withArgs:(NSArray *)data;

- (void) setResourceName:(NSString *)name;

@end