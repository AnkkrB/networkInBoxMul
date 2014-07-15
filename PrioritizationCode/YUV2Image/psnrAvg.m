function avg_psnr = psnrAvg(file1, file2, width, height, numFrames)
    x = tic

    in = loadFileYuv(file1, width, height, 1:numFrames)
    out = loadFileYuv(file2, width, height, 1:numFrames)

    psnr_tot = 0;
    
    for n = 1:numFrames
        psnr_tot = psnr_tot + measerr(in(n).cdata(:,:,:), out(n).cdata(:,:,:));
    end
    
    avg_psnr = psnr_tot / numFrames;

    toc(x)
end