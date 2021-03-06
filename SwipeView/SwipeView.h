//
//  SwipeView.h
//
//  Version 1.0.1
//
//  Created by Nick Lockwood on 03/09/2010.
//  Copyright 2010 Charcoal Design
//
//  Distributed under the permissive zlib License
//  Get the latest version of SwipeView from here:
//
//  https://github.com/nicklockwood/SwipeView
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//  claim that you wrote the original software. If you use this software
//  in a product, an acknowledgment in the product documentation would be
//  appreciated but is not required.
//
//  2. Altered source versions must be plainly marked as such, and must not be
//  misrepresented as being the original software.
//
//  3. This notice may not be removed or altered from any source distribution.
//

//
//  ARC Helper
//
//  Version 2.1
//
//  Created by Nick Lockwood on 05/01/2012.
//  Copyright 2012 Charcoal Design
//
//  Distributed under the permissive zlib license
//  Get the latest version from here:
//
//  https://gist.github.com/1563325
//

#ifndef ah_retain
#if __has_feature(objc_arc)
#define ah_retain self
#define ah_dealloc self
#define release self
#define autorelease self
#else
#define ah_retain retain
#define ah_dealloc dealloc
#define __bridge
#endif
#endif

//  Weak reference support

#import <Availability.h>
#if (defined __IPHONE_OS_VERSION_MIN_REQUIRED && \
__IPHONE_OS_VERSION_MIN_REQUIRED < __IPHONE_5_0) || \
(defined __MAC_OS_X_VERSION_MIN_REQUIRED && \
__MAC_OS_X_VERSION_MIN_REQUIRED < __MAC_10_7)
#undef weak
#define weak unsafe_unretained
#undef __weak
#define __weak __unsafe_unretained
#endif

//  ARC Helper ends


#import <UIKit/UIKit.h>


typedef enum
{
    SwipeViewAlignmentEdge = 0,
    SwipeViewAlignmentCenter
}
SwipeViewAlignment;


@protocol SwipeViewDataSource, SwipeViewDelegate;

@interface SwipeView : UIView

@property (nonatomic, weak) IBOutlet id<SwipeViewDataSource> dataSource;
@property (nonatomic, weak) IBOutlet id<SwipeViewDelegate> delegate;
@property (nonatomic, readonly) NSInteger numberOfItems;
@property (nonatomic, strong, readonly) NSArray *indexesForVisibleItems;
@property (nonatomic, strong, readonly) NSArray *visibleItemViews;
@property (nonatomic, readonly) NSInteger currentItemIndex;
@property (nonatomic, strong, readonly) UIView *currentItemView;
@property (nonatomic, assign) SwipeViewAlignment alignment;
@property (nonatomic, assign, getter = isPagingEnabled) BOOL pagingEnabled;
@property (nonatomic, assign, getter = isScrollEnabled) BOOL scrollEnabled;
@property (nonatomic, assign) BOOL bounces;

- (void)reloadData;
- (void)reloadItemAtIndex:(NSInteger)index;
- (void)scrollByNumberOfItems:(NSInteger)itemCount animated:(BOOL)animated;
- (void)scrollToItemAtIndex:(NSInteger)index animated:(BOOL)animated;
- (UIView *)itemViewAtIndex:(NSInteger)index;
- (NSInteger)indexOfItemView:(UIView *)view;
- (NSInteger)indexOfItemViewOrSubview:(UIView *)view;

@end


@protocol SwipeViewDataSource <NSObject>

- (NSInteger)numberOfItemsInSwipeView:(SwipeView *)swipeView;
- (UIView *)swipeView:(SwipeView *)swipeView viewForItemAtIndex:(NSInteger)index reusingView:(UIView *)view;

@end


@protocol SwipeViewDelegate <NSObject>
@optional

- (void)swipeViewDidScroll:(SwipeView *)swipeView;
- (void)swipeViewCurrentItemIndexDidChange:(SwipeView *)swipeView;
- (void)swipeViewDidEndDragging:(SwipeView *)swipeView willDecelerate:(BOOL)decelerate;
- (void)swipeViewDidEndDecelerating:(SwipeView *)swipeView;
- (BOOL)swipeView:(SwipeView *)swipeView shouldSelectItemAtIndex:(NSInteger)index;
- (void)swipeView:(SwipeView *)swipeView didSelectItemAtIndex:(NSInteger)index;

@end
