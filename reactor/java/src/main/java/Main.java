import reactor.core.publisher.Flux;

public class Main
{
    public static void main(String[] args)
    {
        Flux.range(1, 4)
            .map(i -> { return 2*i; })
            .subscribe(
                System.out::println,
                null,
                () -> System.out.println("OnCompleted")
            );
    }
}
