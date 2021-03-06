Purpose
--------------

SwipeView is a class designed to simplify the implementation of horizontal, paged scrolling views on iOS. It is based on a UIScrollView, but adds convenient functionality such as a UITableView-style dataSource/delegat interface for loading views dynamically, and efficient view loading, unloading and recycling.

SwipeView's interface and implementation is based on the iCarousel library, and should be familiar to anyone who has used iCarousel.


Supported OS & SDK Versions
-----------------------------

* Supported build target - iOS 5.1 / Mac OS 10.7 (Xcode 4.3.2, Apple LLVM compiler 3.1)
* Earliest supported deployment target - iOS 4.3 / Mac OS 10.7
* Earliest compatible deployment target - iOS 3.2 / Mac OS 10.6

NOTE: 'Supported' means that the library has been tested with this version. 'Compatible' means that the library should work on this OS version (i.e. it doesn't rely on any unavailable SDK features) but is no longer being tested for compatibility and may require tweaking or bug fixes to run correctly.


ARC Compatibility
------------------

SwipeView automatically works with both ARC and non-ARC projects through conditional compilation. There is no need to exclude SwipeView files from the ARC validation process, or to convert SwipeView using the ARC conversion tool.


Thread Safety
--------------

SwipeView is derived from UIView and - as with all UIKit components - it should only be accessed from the main thread. You may wish to use threads for loading or updating SwipeView contents or items, but always ensure that once your content has loaded, you switch back to the main thread before updating the SwipeView.


Installation
--------------

To use the SwipeView class in an app, just drag the SwipeView class files (demo files and assets are not needed) into your project.


Properties
--------------

The SwipeView has the following properties:

	@property (nonatomic, weak) IBOutlet id<SwipeViewDataSource> dataSource;

An object that supports the SwipeViewDataSource protocol and can provide views to populate the SwipeView.

	@property (nonatomic, weak) IBOutlet id<SwipeViewDelegate> delegate;

An object that supports the SwipeViewDelegate protocol and can respond to SwipeView events and layout requests.

    @property (nonatomic, readonly) NSInteger numberOfItems;
    
The number of items in the SwipeView (read only). To set this, implement the `numberOfItemsInSwipeView:` dataSource method. Note that not all of these item views will be loaded or visible at a given point in time - the SwipeView loads item views on demand as it scrolls.
    
    @property (nonatomic, strong, readonly) NSArray *indexesForVisibleItems;
	
An array containing the indexes of all item views currently loaded and visible in the SwipeView. The array contains NSNumber objects whose integer values match the indexes of the views. The indexes for item views start at zero and match the indexes passed to the dataSource to load the view.

	@property (nonatomic, strong, readonly) NSArray *visibleItemViews;

An array of all the item views currently displayed in the SwipeView (read only). The indexes of views in this array do not match the item indexes, however the order of these views matches the order of the visibleItemIndexes array property, i.e. you can get the item index of a given view in this array by retrieving the equivalent object from the visibleItemIndexes array (or, you can just use the `indexOfItemView:` method, which is much easier).
    
    @property (nonatomic, assign) SwipeViewAlignment alignment;
    
This property controls how the SwipeView items are aligned. The default value of `SwipeViewAlignmentEdge` means that the item views will extend to the edges of the SwipeView. Switching the alignment to `SwipeViewAlignmentCenter` means that the leftmost and rightmost item views will be centered when the SwipeView is fully scrolled to either extreme.
    
    @property (nonatomic, readonly) NSInteger currentItemIndex;
    
The index of the currently centered (or left-aligned, depending on the alignment value) item view.
    
    @property (nonatomic, strong, readonly) UIView *currentItemView;
    
The currently centered (or left-aligned, depending on the alignment value) item view.
    
    @property (nonatomic, assign, getter = isPagingEnabled) BOOL pagingEnabled;
    
Enables and disables paging. When paging is enabled, the SwipeView will stop at each item view as the user scrolls.
    
    @property (nonatomic, assign, getter = isScrollEnabled) BOOL scrollEnabled;

Enables and disables user scrolling of the SwipeView. The SwipeView can still be scrolled programmatically if this property is set to NO.

	@property (nonatomic, assign) BOOL bounces;

Sets whether the SwipeView should bounce past the end and return, or stop dead.

	@property (nonatomic, assign) BOOL clipsToBounds;
	
This is actually not a property of SwipeView but is inherited from UIView. It's included here because it's a frequently missed feature. Set this to YES to prevent the swipeView item views overflowing their bounds. You can set this property in Interface Builder by ticking the 'Clip Subviews' option. Defaults to NO.

	
Methods
--------------

The SwipeView class has the following methods:

	- (void)reloadData;

This reloads all SwipeView item views from the dataSource and refreshes the display.

	- (void)reloadItemAtIndex:(NSInteger)index;
	
This method will reload the specified item view. The new item will be requested from the dataSource.

	- (void)scrollToItemAtIndex:(NSInteger)index animated:(BOOL)animated;

This will center the SwipeView on the specified item, either immediately or with a smooth animation.

	- (void)scrollByNumberOfItems:(NSInteger)itemCount animated:(BOOL)animated;

This method allows you to scroll the SwipeView by a fixed distance, measured in item widths. Positive or negative values may be specified for itemCount, depending on the direction you wish to scroll. SwipeView gracefully handles bounds issues, so if you specify a distance greater than the number of items in the SwipeView, scrolling will be clamped when it reaches the end of the SwipeView.

	- (UIView *)itemViewAtIndex:(NSInteger)index;
	
Returns the visible item view with the specified index. Note that the index relates to the position in the SwipeView, and not the position in the `visibleItemViews` array, which may be different. The method only works for visible item views and will return nil if the view at the specified index has not been loaded, or if the index is out of bounds.

	- (NSInteger)indexOfItemView:(UIView *)view;
	
The index for a given item view in the SwipeView. This method only works for visible item views and will return NSNotFound for views that are not currently loaded. For a list of all currently loaded views, use the `visibleItemViews` property.

	- (NSInteger)indexOfItemViewOrSubview:(UIView *)view

This method gives you the item index of either the view passed or the view containing the view passed as a parameter. It works by walking up the view hierarchy starting with the view passed until it finds an item view and returns its index within the SwipeView. If no currently-loaded item view is found, it returns NSNotFound. This method is extremely useful for handling events on controls embedded within an item view. This allows you to bind all your item controls to a single action method on your view controller, and then work out which item the control that triggered the action was related to.


Protocols
---------------

The SwipeView follows the Apple convention for data-driven views by providing two protocol interfaces, SwipeViewDataSource and SwipeViewDelegate. The SwipeViewDataSource protocol has the following required methods:

	- (NSUInteger)numberOfItemsInSwipeView:(SwipeView *)swipeView;

Return the number of items (views) in the SwipeView.

	- (UIView *)swipeView:(SwipeView *)swipeView viewForItemAtIndex:(NSUInteger)index reusingView:(UIView *)view;

Return a view to be displayed at the specified index in the SwipeView. The `reusingView` argument works like a UIPickerView, where views that have previously been displayed in the SwipeView are passed back to the method to be recycled. If this argument is not nil, you can set its properties and return it instead of creating a new view instance, which will slightly improve performance. Unlike UITableView, there is no reuseIdentifier for distinguishing between different SwipeView view types, so if your SwipeView contains multiple different view types then you should just ignore this parameter and return a new view each time the method is called. You should ensure that each time the `swipeView:viewForItemAtIndex:reusingView:` method is called, it either returns the reusingView or a brand new view instance rather than maintaining your own pool of recyclable views, as returning multiple copies of the same view for different SwipeView item indexes may cause display issues with the SwipeView.

The SwipeViewDelegate protocol has the following optional methods:

    - (void)swipeViewDidScroll:(SwipeView *)swipeView;
    
This method is called whenever the SwipeView is scrolled. It is called regardless of whether the SwipeView was scrolled programatically or through user interaction.
    
    - (void)swipeViewCurrentItemIndexDidChange:(SwipeView *)swipeView;
    
This method is called whenever the SwipeView scrolls far enough for the currentItemIndex property to change. It is called regardless of whether the item index was updated programatically or through user interaction.
    
    - (void)swipeViewDidEndDragging:(SwipeView *)swipeView willDecelerate:(BOOL)decelerate;
    
This method is called when the user stops dragging the SwipeView. The willDecelerate parameter indicates whether the SwipeView is travelling fast enough that it needs to decelerate before it stops (i.e. the current index is not necessarily the one it will stop at) or if it will stop where it is. Note that even if willDecelerate is NO, the SwipeView will still scroll automatically until it aligns exactly on the current index.
    
    - (void)swipeViewDidEndDecelerating:(SwipeView *)swipeView;
    
This method is called when the SwipeView finishes decelerating and you can assume that the currentItemIndex at this point is the final stopping value.
    
    - (void)swipeView:(SwipeView *)swipeView didSelectItemAtIndex:(NSInteger)index;

This method will fire if the user taps any SwipeView item view. This method will not fire if the user taps a control within the currently selected view (i.e. any view that is a subclass of UIControl).

	- (BOOL)swipeView:(SwipeView *)swipeView shouldSelectItemAtIndex:(NSInteger)index;
	
This method will fire if the user taps any SwipeView item view. The purpose of a method is to give you the opportunity to ignore a tap on the SwipeView. If you return YES from the method, or don't implement it, the tap will be processed as normal and the `swipeView:didSelectItemAtIndex:` method will be called. If you return NO, the SwipeView will ignore the tap and it will continue to propagate up the view hierarchy. This is a good way to prevent the SwipeView intercepting tap events intended for processing by another view.


Detecting Taps on Item Views
----------------------------

There are two basic approaches to detecting taps on views in SwipeView. The first approach is to simply use the `swipeView:didSelectItemAtIndex:` delegate method, which fires every time an item is tapped.

Alternatively, if you want a little more control you can supply a UIButton or UIControl as the item view and handle the touch interactions yourself.

You can also nest UIControls within your item views and these will receive touches as expected.

If you wish to detect other types of interaction such as swipes, double taps or long presses, the simplest way is to attach a UIGestureRecognizer to your item view or its subviews before passing it to the SwipeView.