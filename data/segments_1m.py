import open3d as o3d

# sample data from https://github.com/artem-ogre/CDT/issues/10
pcd = o3d.io.read_point_cloud('segments.pcd')
pcd = pcd.voxel_down_sample(1.0)
o3d.io.write_point_cloud('segments_1m.pcd', pcd)
