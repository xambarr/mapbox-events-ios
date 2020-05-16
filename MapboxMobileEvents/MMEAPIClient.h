#import <Foundation/Foundation.h>
#import "MMETypes.h"

NS_ASSUME_NONNULL_BEGIN

@class MMEConfig;
@class MMEEvent;
@class MMENSURLRequestFactory;
@class MMENSURLSessionWrapper;
@protocol MMEEventConfigProviding;

// MARK: - Types


typedef void(^OnEventQueueUpdate)(NSArray * eventQueue);
typedef void(^OnEventCountUpdate)(NSUInteger eventCount, NSURLRequest* _Nullable request, NSError * _Nullable error);
typedef void(^OnGenerateTelemetryEvent)(void);

/*!
 @Brief Mapbox API Abstraction
 @Discussion MMEAPIClient provides root network setup as well as asynchronous api call abstractions
 @Note Completion blocks will not be called if Client is not retained
 */
@interface MMEAPIClient : NSObject

// MARK: - Properties

/*! @brief Configuration Providing Shared Values for constructing Requests */
@property (nonatomic, readonly) id<MMEEventConfigProviding> config;


// MARK: - Initializers

- (instancetype)init NS_UNAVAILABLE;

/*! @Brief Default Client Setup */
- (instancetype)initWithConfig:(id <MMEEventConfigProviding>)config;

/*! @Brief Initializes a Client with a custom session wrapper without event hooks
 @param config Provider of Shared Client Model Information
 @param session Session responsible for owning URLSession and Cert pinning
 */
- (instancetype)initWithConfig:(id <MMEEventConfigProviding>)config
                       session:(MMENSURLSessionWrapper*)session;

/*! @Brief Initializes a Client with hooks and default sesion setup
 @param config Provider of Shared Client Model Information
 @param onSerializationError Called upon serialization or deserialization errors
 @param onURLResponse Called on URL Responses (useful for analytics/general http error logging).
 @param onEventQueueUpdate Called on the EventQueue updates
 @param onEventCountUpdate Called on the EventCount Udpates
 @param onGenerateTelemetryEvent Called on the Generation of Telemetry Events
 */

- (instancetype)initWithConfig:(id <MMEEventConfigProviding>)config
          onSerializationError:(OnSerializationError)onSerializationError
                onURLResponse:(OnURLResponse)onURLResponse
            onEventQueueUpdate:(OnEventQueueUpdate)onEventQueueUpdate
            onEventCountUpdate:(OnEventCountUpdate)onEventCountUpdate
      onGenerateTelemetryEvent:(OnGenerateTelemetryEvent)onGenerateTelemetryEvent;

/** Designated Initializer
 @param config Provider of Shared Client Model Information
 @param requestFactory Factory Responsibile for building requests
 @param session Session responsible for owning URLSession and Cert pinning
 @param onSerializationError Called upon serialization or deserialization errors
 @param onURLResponse Called on URL Responses (useful for analytics/general http error logging)
 @param onEventQueueUpdate Called on the EventQueue updates
 @param onEventCountUpdate Called on the EventCount Udpates
 @param onGenerateTelemetryEvent Called on the Generation of Telemetry Events
 */
- (instancetype)initWithConfig:(id <MMEEventConfigProviding>)config
                requestFactory:(MMENSURLRequestFactory*)requestFactory
                       session:(MMENSURLSessionWrapper*)session
          onSerializationError:(OnSerializationError)onSerializationError
                onURLResponse:(OnURLResponse)onURLResponse
            onEventQueueUpdate:(OnEventQueueUpdate)onEventQueueUpdate
            onEventCountUpdate:(OnEventCountUpdate)onEventCountUpdate
      onGenerateTelemetryEvent:(OnGenerateTelemetryEvent)onGenerateTelemetryEvent NS_DESIGNATED_INITIALIZER;


// MARK: - Requests

/**
 @Brief Designated Perform Request
 @Discussion All Requests should fall through this function for general tracking of network metrics
 @param request URLRequest to be performed
 @param completion Asynchronous Completion Handler
 */
- (void)performRequest:(NSURLRequest*)request
     completion:(nullable void (^)(NSData * _Nullable data, NSHTTPURLResponse * _Nullable response, NSError * _Nullable error))completion;

// MARK: - Events Service

/*!
 @Brief Track a single event
 @param event Event to track
 @param completionHandler Completion event with optional error
 */
- (void)postEvent:(MMEEvent *)event completionHandler:(nullable void (^)(NSError * _Nullable error))completionHandler;

/**
 @Brief Track a single event
 @param events Array of events to track
 @param completionHandler Completion event with optional error
 */
- (void)postEvents:(NSArray <MMEEvent*> *)events completionHandler:(nullable void (^)(NSError * _Nullable error))completionHandler;


- (void)postMetadata:(NSArray *)metadata
           filePaths:(NSArray *)filePaths
   completionHandler:(nullable void (^)(NSError * _Nullable error))completionHandler;


// MARK: - Configuration Service

/** Fetch Event Config (Service Driven Behavior Reporting)
 @param completion Block called at the end of network operation (Result being JSON Object or NSError)
 */
- (void)getEventConfigWithCompletionHandler:(nullable void (^)(MMEConfig* _Nullable config, NSError * _Nullable error))completion;

@end

NS_ASSUME_NONNULL_END
