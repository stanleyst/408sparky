import numpy as np
import cv2
from matplotlib import pyplot as plt

MIN_MATCH_COUNT = 3
FLANN_INDEX_KDTREE = 0


def run_frame_processing(image_1, image_2):
    # Open the target image and the scene image
    img = cv2.imread(image_1, 0)
    scene = cv2.imread(image_2, 0)

    target = cv2.Canny(img,0,500)

    orb = cv2.ORB_create()

    kp_1, des_1 = orb.detectAndCompute(target, None)
    kp_2, des_2 = orb.detectAndCompute(scene, None)


    FLANN_INDEX_LSH = 6

    index_params= dict(algorithm = FLANN_INDEX_LSH,
            table_number = 6, # 12
            key_size = 12,     # 20
            multi_probe_level = 1) #2

    search_params = dict(checks = 50)
    flann = cv2.FlannBasedMatcher(index_params, search_params)

    matches = flann.knnMatch(des_1,des_2,k=2)

    '''
    bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)

    matches = bf.match(des_1, des_2)

    matches = sorted(matches, key = lambda x:x.distance)

    image = cv2.drawMatches(target, kp_1, scene, kp_2, matches[:10], None, flags=2)

    plt.imshow(image)
    plt.show()
    '''

    '''
    index_params = dict(algorithm = FLANN_INDEX_KDTREE, trees=5)
    search_params = dict(checks=50)

    flann = cv2.FlannBasedMatcher(index_params, search_params)

    matches = flann.knnMatch(des_1, des_2, k=2)
    good = []
    for m,n in matches:
        if m.distance < 0.7*n.distance:
            good.append(m)


    '''

    good = []
    for m_n in matches:
        if len(m_n) != 2:
            continue
        (m,n) = m_n
        if m.distance < 0.7*n.distance:
            good.append(m)

    if len(good) > MIN_MATCH_COUNT:
        src_pts = np.float32([ kp_1[m.queryIdx].pt for m in good ]).reshape(-1,1,2)
        dst_pts = np.float32([ kp_2[m.trainIdx].pt for m in good ]).reshape(-1,1,2)

        M, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 5.0)
        matches_mask = mask.ravel().tolist()

        h,w = target.shape
        pts = np.float32([ [0,0],[0,h-1],[w-1,h-1],[w-1,0] ]).reshape(-1,1,2)
        dst = cv2.perspectiveTransform(pts,M)

        scene = cv2.polylines(scene, [np.int32(dst)], True, 155, 3, cv2.LINE_AA)

    else:
        print('Not enough good matches. {} found'.format(len(good)))
        matches_mask = None


    draw_params = dict(matchColor = (255,0,0),
        singlePointColor = None,
        matchesMask = matches_mask,
        flags = 2)

    img_3 = cv2.drawMatches(target, kp_1, scene, kp_2, good, None, **draw_params)

    pixels = [kp_2[mat.trainIdx].pt for mat in good]

    print('The pixels in the second image are: {}'.format(pixels))

    height, width = scene.shape

    print('Pixel height is: {}'.format(height))
    print('Pixel width is: {}'.format(width))

    marker = width/4
    left = right = straight = 0

    r = l = s = False

    longt = [x[0] for x in pixels]

    for point in longt:
        if point <= marker:
            left += 1
        elif point >= (marker*3):
            right += 1
        else:
            straight += 1

    if (left >= right) and (left >= straight):
        print('Turning left')
        l = True
    elif (right >= left) and (right >= straight):
        print('Turning right')
        r = True
    else:
        print('Going Straight')
        s = True

    plt.imshow(img_3, 'gray'), plt.show()


    return r, l, s

if __name__ == '__main__':
    run_frame_processing('star.jpg', 'better_zak.jpg')
