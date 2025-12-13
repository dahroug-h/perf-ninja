; ModuleID = 'solution.cpp'
source_filename = "solution.cpp"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx16.0.0"

%"struct.std::__1::array" = type { [16 x i16] }
%"struct.std::__1::array.1" = type { [201 x i16] }
%"struct.std::__1::array.0" = type { [200 x i8] }

; Function Attrs: mustprogress nofree norecurse nosync nounwind ssp memory(read, argmem: readwrite, inaccessiblemem: none) uwtable(sync)
define void @_Z17compute_alignmentRKNSt3__16vectorINS_5arrayIhLm200EEENS_9allocatorIS2_EEEES7_(ptr dead_on_unwind noalias nocapture writable writeonly sret(%"struct.std::__1::array") align 2 initializes((0, 32)) %0, ptr nocapture noundef nonnull readonly align 8 dereferenceable(24) %1, ptr nocapture noundef nonnull readonly align 8 dereferenceable(24) %2) local_unnamed_addr #0 {
  %4 = alloca %"struct.std::__1::array.1", align 2
  %5 = alloca %"struct.std::__1::array.1", align 2
  tail call void @llvm.memset.p0.i64(ptr noundef nonnull align 2 dereferenceable(32) %0, i8 0, i64 32, i1 false)
  %6 = getelementptr inbounds nuw i8, ptr %1, i64 8
  %7 = load ptr, ptr %6, align 8, !tbaa !5
  %8 = load ptr, ptr %1, align 8, !tbaa !11
  %9 = icmp eq ptr %7, %8
  br i1 %9, label %68, label %10

10:                                               ; preds = %3
  %11 = ptrtoint ptr %7 to i64
  %12 = ptrtoint ptr %8 to i64
  %13 = sub i64 %11, %12
  %14 = sdiv exact i64 %13, 200
  %15 = load ptr, ptr %2, align 8, !tbaa !11
  %16 = getelementptr inbounds nuw i8, ptr %4, i64 400
  %17 = tail call i64 @llvm.umax.i64(i64 %14, i64 1)
  %18 = getelementptr inbounds nuw i8, ptr %4, i64 2
  %19 = getelementptr inbounds nuw i8, ptr %4, i64 18
  %20 = getelementptr inbounds nuw i8, ptr %4, i64 34
  %21 = getelementptr inbounds nuw i8, ptr %4, i64 50
  %22 = getelementptr inbounds nuw i8, ptr %5, i64 2
  %23 = getelementptr inbounds nuw i8, ptr %5, i64 18
  %24 = getelementptr inbounds nuw i8, ptr %5, i64 34
  %25 = getelementptr inbounds nuw i8, ptr %5, i64 50
  %26 = getelementptr inbounds nuw i8, ptr %4, i64 66
  %27 = getelementptr inbounds nuw i8, ptr %4, i64 82
  %28 = getelementptr inbounds nuw i8, ptr %4, i64 98
  %29 = getelementptr inbounds nuw i8, ptr %4, i64 114
  %30 = getelementptr inbounds nuw i8, ptr %5, i64 66
  %31 = getelementptr inbounds nuw i8, ptr %5, i64 82
  %32 = getelementptr inbounds nuw i8, ptr %5, i64 98
  %33 = getelementptr inbounds nuw i8, ptr %5, i64 114
  %34 = getelementptr inbounds nuw i8, ptr %4, i64 130
  %35 = getelementptr inbounds nuw i8, ptr %4, i64 146
  %36 = getelementptr inbounds nuw i8, ptr %4, i64 162
  %37 = getelementptr inbounds nuw i8, ptr %4, i64 178
  %38 = getelementptr inbounds nuw i8, ptr %5, i64 130
  %39 = getelementptr inbounds nuw i8, ptr %5, i64 146
  %40 = getelementptr inbounds nuw i8, ptr %5, i64 162
  %41 = getelementptr inbounds nuw i8, ptr %5, i64 178
  %42 = getelementptr inbounds nuw i8, ptr %4, i64 194
  %43 = getelementptr inbounds nuw i8, ptr %4, i64 210
  %44 = getelementptr inbounds nuw i8, ptr %4, i64 226
  %45 = getelementptr inbounds nuw i8, ptr %4, i64 242
  %46 = getelementptr inbounds nuw i8, ptr %5, i64 194
  %47 = getelementptr inbounds nuw i8, ptr %5, i64 210
  %48 = getelementptr inbounds nuw i8, ptr %5, i64 226
  %49 = getelementptr inbounds nuw i8, ptr %5, i64 242
  %50 = getelementptr inbounds nuw i8, ptr %4, i64 258
  %51 = getelementptr inbounds nuw i8, ptr %4, i64 274
  %52 = getelementptr inbounds nuw i8, ptr %4, i64 290
  %53 = getelementptr inbounds nuw i8, ptr %4, i64 306
  %54 = getelementptr inbounds nuw i8, ptr %5, i64 258
  %55 = getelementptr inbounds nuw i8, ptr %5, i64 274
  %56 = getelementptr inbounds nuw i8, ptr %5, i64 290
  %57 = getelementptr inbounds nuw i8, ptr %5, i64 306
  %58 = getelementptr inbounds nuw i8, ptr %4, i64 322
  %59 = getelementptr inbounds nuw i8, ptr %4, i64 338
  %60 = getelementptr inbounds nuw i8, ptr %4, i64 354
  %61 = getelementptr inbounds nuw i8, ptr %4, i64 370
  %62 = getelementptr inbounds nuw i8, ptr %5, i64 322
  %63 = getelementptr inbounds nuw i8, ptr %5, i64 338
  %64 = getelementptr inbounds nuw i8, ptr %5, i64 354
  %65 = getelementptr inbounds nuw i8, ptr %5, i64 370
  %66 = getelementptr inbounds nuw i8, ptr %4, i64 386
  %67 = getelementptr inbounds nuw i8, ptr %5, i64 386
  br label %69

68:                                               ; preds = %73, %3
  ret void

69:                                               ; preds = %10, %73
  %70 = phi i64 [ 0, %10 ], [ %76, %73 ]
  %71 = getelementptr inbounds nuw %"struct.std::__1::array.0", ptr %8, i64 %70
  call void @llvm.lifetime.start.p0(i64 402, ptr nonnull %4) #4
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 2 dereferenceable(402) %4, i8 0, i64 402, i1 false)
  call void @llvm.lifetime.start.p0(i64 402, ptr nonnull %5) #4
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 2 dereferenceable(402) %5, i8 0, i64 402, i1 false)
  store <8 x i16> <i16 -11, i16 -12, i16 -13, i16 -14, i16 -15, i16 -16, i16 -17, i16 -18>, ptr %18, align 2, !tbaa !12
  store <8 x i16> <i16 -19, i16 -20, i16 -21, i16 -22, i16 -23, i16 -24, i16 -25, i16 -26>, ptr %19, align 2, !tbaa !12
  store <8 x i16> <i16 -27, i16 -28, i16 -29, i16 -30, i16 -31, i16 -32, i16 -33, i16 -34>, ptr %20, align 2, !tbaa !12
  store <8 x i16> <i16 -35, i16 -36, i16 -37, i16 -38, i16 -39, i16 -40, i16 -41, i16 -42>, ptr %21, align 2, !tbaa !12
  store <8 x i16> <i16 -22, i16 -23, i16 -24, i16 -25, i16 -26, i16 -27, i16 -28, i16 -29>, ptr %22, align 2, !tbaa !12
  store <8 x i16> <i16 -30, i16 -31, i16 -32, i16 -33, i16 -34, i16 -35, i16 -36, i16 -37>, ptr %23, align 2, !tbaa !12
  store <8 x i16> <i16 -38, i16 -39, i16 -40, i16 -41, i16 -42, i16 -43, i16 -44, i16 -45>, ptr %24, align 2, !tbaa !12
  store <8 x i16> <i16 -46, i16 -47, i16 -48, i16 -49, i16 -50, i16 -51, i16 -52, i16 -53>, ptr %25, align 2, !tbaa !12
  store <8 x i16> <i16 -43, i16 -44, i16 -45, i16 -46, i16 -47, i16 -48, i16 -49, i16 -50>, ptr %26, align 2, !tbaa !12
  store <8 x i16> <i16 -51, i16 -52, i16 -53, i16 -54, i16 -55, i16 -56, i16 -57, i16 -58>, ptr %27, align 2, !tbaa !12
  store <8 x i16> <i16 -59, i16 -60, i16 -61, i16 -62, i16 -63, i16 -64, i16 -65, i16 -66>, ptr %28, align 2, !tbaa !12
  store <8 x i16> <i16 -67, i16 -68, i16 -69, i16 -70, i16 -71, i16 -72, i16 -73, i16 -74>, ptr %29, align 2, !tbaa !12
  store <8 x i16> <i16 -54, i16 -55, i16 -56, i16 -57, i16 -58, i16 -59, i16 -60, i16 -61>, ptr %30, align 2, !tbaa !12
  store <8 x i16> <i16 -62, i16 -63, i16 -64, i16 -65, i16 -66, i16 -67, i16 -68, i16 -69>, ptr %31, align 2, !tbaa !12
  store <8 x i16> <i16 -70, i16 -71, i16 -72, i16 -73, i16 -74, i16 -75, i16 -76, i16 -77>, ptr %32, align 2, !tbaa !12
  store <8 x i16> <i16 -78, i16 -79, i16 -80, i16 -81, i16 -82, i16 -83, i16 -84, i16 -85>, ptr %33, align 2, !tbaa !12
  store <8 x i16> <i16 -75, i16 -76, i16 -77, i16 -78, i16 -79, i16 -80, i16 -81, i16 -82>, ptr %34, align 2, !tbaa !12
  store <8 x i16> <i16 -83, i16 -84, i16 -85, i16 -86, i16 -87, i16 -88, i16 -89, i16 -90>, ptr %35, align 2, !tbaa !12
  store <8 x i16> <i16 -91, i16 -92, i16 -93, i16 -94, i16 -95, i16 -96, i16 -97, i16 -98>, ptr %36, align 2, !tbaa !12
  store <8 x i16> <i16 -99, i16 -100, i16 -101, i16 -102, i16 -103, i16 -104, i16 -105, i16 -106>, ptr %37, align 2, !tbaa !12
  store <8 x i16> <i16 -86, i16 -87, i16 -88, i16 -89, i16 -90, i16 -91, i16 -92, i16 -93>, ptr %38, align 2, !tbaa !12
  store <8 x i16> <i16 -94, i16 -95, i16 -96, i16 -97, i16 -98, i16 -99, i16 -100, i16 -101>, ptr %39, align 2, !tbaa !12
  store <8 x i16> <i16 -102, i16 -103, i16 -104, i16 -105, i16 -106, i16 -107, i16 -108, i16 -109>, ptr %40, align 2, !tbaa !12
  store <8 x i16> <i16 -110, i16 -111, i16 -112, i16 -113, i16 -114, i16 -115, i16 -116, i16 -117>, ptr %41, align 2, !tbaa !12
  store <8 x i16> <i16 -107, i16 -108, i16 -109, i16 -110, i16 -111, i16 -112, i16 -113, i16 -114>, ptr %42, align 2, !tbaa !12
  store <8 x i16> <i16 -115, i16 -116, i16 -117, i16 -118, i16 -119, i16 -120, i16 -121, i16 -122>, ptr %43, align 2, !tbaa !12
  store <8 x i16> <i16 -123, i16 -124, i16 -125, i16 -126, i16 -127, i16 -128, i16 -129, i16 -130>, ptr %44, align 2, !tbaa !12
  store <8 x i16> <i16 -131, i16 -132, i16 -133, i16 -134, i16 -135, i16 -136, i16 -137, i16 -138>, ptr %45, align 2, !tbaa !12
  store <8 x i16> <i16 -118, i16 -119, i16 -120, i16 -121, i16 -122, i16 -123, i16 -124, i16 -125>, ptr %46, align 2, !tbaa !12
  store <8 x i16> <i16 -126, i16 -127, i16 -128, i16 -129, i16 -130, i16 -131, i16 -132, i16 -133>, ptr %47, align 2, !tbaa !12
  store <8 x i16> <i16 -134, i16 -135, i16 -136, i16 -137, i16 -138, i16 -139, i16 -140, i16 -141>, ptr %48, align 2, !tbaa !12
  store <8 x i16> <i16 -142, i16 -143, i16 -144, i16 -145, i16 -146, i16 -147, i16 -148, i16 -149>, ptr %49, align 2, !tbaa !12
  store <8 x i16> <i16 -139, i16 -140, i16 -141, i16 -142, i16 -143, i16 -144, i16 -145, i16 -146>, ptr %50, align 2, !tbaa !12
  store <8 x i16> <i16 -147, i16 -148, i16 -149, i16 -150, i16 -151, i16 -152, i16 -153, i16 -154>, ptr %51, align 2, !tbaa !12
  store <8 x i16> <i16 -155, i16 -156, i16 -157, i16 -158, i16 -159, i16 -160, i16 -161, i16 -162>, ptr %52, align 2, !tbaa !12
  store <8 x i16> <i16 -163, i16 -164, i16 -165, i16 -166, i16 -167, i16 -168, i16 -169, i16 -170>, ptr %53, align 2, !tbaa !12
  store <8 x i16> <i16 -150, i16 -151, i16 -152, i16 -153, i16 -154, i16 -155, i16 -156, i16 -157>, ptr %54, align 2, !tbaa !12
  store <8 x i16> <i16 -158, i16 -159, i16 -160, i16 -161, i16 -162, i16 -163, i16 -164, i16 -165>, ptr %55, align 2, !tbaa !12
  store <8 x i16> <i16 -166, i16 -167, i16 -168, i16 -169, i16 -170, i16 -171, i16 -172, i16 -173>, ptr %56, align 2, !tbaa !12
  store <8 x i16> <i16 -174, i16 -175, i16 -176, i16 -177, i16 -178, i16 -179, i16 -180, i16 -181>, ptr %57, align 2, !tbaa !12
  store <8 x i16> <i16 -171, i16 -172, i16 -173, i16 -174, i16 -175, i16 -176, i16 -177, i16 -178>, ptr %58, align 2, !tbaa !12
  store <8 x i16> <i16 -179, i16 -180, i16 -181, i16 -182, i16 -183, i16 -184, i16 -185, i16 -186>, ptr %59, align 2, !tbaa !12
  store <8 x i16> <i16 -187, i16 -188, i16 -189, i16 -190, i16 -191, i16 -192, i16 -193, i16 -194>, ptr %60, align 2, !tbaa !12
  store <8 x i16> <i16 -195, i16 -196, i16 -197, i16 -198, i16 -199, i16 -200, i16 -201, i16 -202>, ptr %61, align 2, !tbaa !12
  store <8 x i16> <i16 -182, i16 -183, i16 -184, i16 -185, i16 -186, i16 -187, i16 -188, i16 -189>, ptr %62, align 2, !tbaa !12
  store <8 x i16> <i16 -190, i16 -191, i16 -192, i16 -193, i16 -194, i16 -195, i16 -196, i16 -197>, ptr %63, align 2, !tbaa !12
  store <8 x i16> <i16 -198, i16 -199, i16 -200, i16 -201, i16 -202, i16 -203, i16 -204, i16 -205>, ptr %64, align 2, !tbaa !12
  store <8 x i16> <i16 -206, i16 -207, i16 -208, i16 -209, i16 -210, i16 -211, i16 -212, i16 -213>, ptr %65, align 2, !tbaa !12
  store <8 x i16> <i16 -203, i16 -204, i16 -205, i16 -206, i16 -207, i16 -208, i16 -209, i16 -210>, ptr %66, align 2, !tbaa !12
  store <8 x i16> <i16 -214, i16 -215, i16 -216, i16 -217, i16 -218, i16 -219, i16 -220, i16 -221>, ptr %67, align 2, !tbaa !12
  %72 = getelementptr inbounds nuw %"struct.std::__1::array.0", ptr %15, i64 %70
  br label %78

73:                                               ; preds = %86
  %74 = load i16, ptr %16, align 2, !tbaa !12
  %75 = getelementptr inbounds nuw [16 x i16], ptr %0, i64 0, i64 %70
  store i16 %74, ptr %75, align 2, !tbaa !12
  call void @llvm.lifetime.end.p0(i64 402, ptr nonnull %5) #4
  call void @llvm.lifetime.end.p0(i64 402, ptr nonnull %4) #4
  %76 = add nuw i64 %70, 1
  %77 = icmp eq i64 %76, %17
  br i1 %77, label %68, label %69, !llvm.loop !14

78:                                               ; preds = %69, %86
  %79 = phi i64 [ 1, %69 ], [ %88, %86 ]
  %80 = phi i16 [ 0, %69 ], [ %81, %86 ]
  %81 = phi i16 [ -11, %69 ], [ %87, %86 ]
  %82 = add i16 %81, -11
  %83 = add nsw i64 %79, -1
  %84 = getelementptr inbounds nuw [200 x i8], ptr %72, i64 0, i64 %83
  %85 = load i8, ptr %84, align 1, !tbaa !16
  br label %90

86:                                               ; preds = %90
  %87 = add i16 %81, -1
  %88 = add nuw nsw i64 %79, 1
  %89 = icmp eq i64 %88, 201
  br i1 %89, label %73, label %78, !llvm.loop !17

90:                                               ; preds = %78, %90
  %91 = phi i64 [ 1, %78 ], [ %111, %90 ]
  %92 = phi i16 [ %80, %78 ], [ %105, %90 ]
  %93 = phi i16 [ %82, %78 ], [ %109, %90 ]
  %94 = add nsw i64 %91, -1
  %95 = getelementptr inbounds nuw [200 x i8], ptr %71, i64 0, i64 %94
  %96 = load i8, ptr %95, align 1, !tbaa !16
  %97 = icmp eq i8 %96, %85
  %98 = select i1 %97, i16 6, i16 -4
  %99 = add i16 %98, %92
  %100 = tail call i16 @llvm.smax.i16(i16 %99, i16 %93)
  %101 = getelementptr inbounds nuw [201 x i16], ptr %5, i64 0, i64 %91
  %102 = load i16, ptr %101, align 2, !tbaa !12
  %103 = tail call i16 @llvm.smax.i16(i16 %100, i16 %102)
  %104 = getelementptr inbounds nuw [201 x i16], ptr %4, i64 0, i64 %91
  %105 = load i16, ptr %104, align 2, !tbaa !12
  store i16 %103, ptr %104, align 2, !tbaa !12
  %106 = add i16 %103, -11
  %107 = add i16 %93, -1
  %108 = add i16 %102, -1
  %109 = tail call i16 @llvm.smax.i16(i16 %107, i16 %106)
  %110 = tail call i16 @llvm.smax.i16(i16 %108, i16 %106)
  store i16 %110, ptr %101, align 2, !tbaa !12
  %111 = add nuw nsw i64 %91, 1
  %112 = icmp eq i64 %111, 201
  br i1 %112, label %86, label %90, !llvm.loop !18
}

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #1

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #2

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr nocapture) #2

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i16 @llvm.smax.i16(i16, i16) #3

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i64 @llvm.umax.i64(i64, i64) #3

attributes #0 = { mustprogress nofree norecurse nosync nounwind ssp memory(read, argmem: readwrite, inaccessiblemem: none) uwtable(sync) "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+ccpp,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" }
attributes #1 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #2 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #3 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #4 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 1}
!3 = !{i32 7, !"frame-pointer", i32 1}
!4 = !{!"Homebrew clang version 20.1.5"}
!5 = !{!6, !7, i64 8}
!6 = !{!"_ZTSNSt3__16vectorINS_5arrayIhLm200EEENS_9allocatorIS2_EEEE", !7, i64 0, !7, i64 8, !7, i64 16}
!7 = !{!"p1 _ZTSNSt3__15arrayIhLm200EEE", !8, i64 0}
!8 = !{!"any pointer", !9, i64 0}
!9 = !{!"omnipotent char", !10, i64 0}
!10 = !{!"Simple C++ TBAA"}
!11 = !{!6, !7, i64 0}
!12 = !{!13, !13, i64 0}
!13 = !{!"short", !9, i64 0}
!14 = distinct !{!14, !15}
!15 = !{!"llvm.loop.mustprogress"}
!16 = !{!9, !9, i64 0}
!17 = distinct !{!17, !15}
!18 = distinct !{!18, !15}
